#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include <algorithm>
#include "welzl.hpp"

using namespace std;

void run_random_experiments() {
    vector<int> sizes = {1, 2, 3, 4, 10, 100, 1000, 10000, 50000, 100000, 200000, 500000, 1000000};
    int iterations = 100;

    ofstream out1("data/exp1_size_vs_time.csv");
    out1 << "Size,AvgTime_ms\n";

    ofstream out3("data/exp3_size_vs_boundary.csv");
    out3 << "Size,AvgBoundaryPoints\n";

    mt19937 gen(1337);
    uniform_real_distribution<double> dist_p(-1000.0, 1000.0);

    for (int n : sizes) {
        cout << "[Exp 1 & 3] Running for N = " << n << "... " << flush;
        double total_time = 0;
        double total_boundary = 0;

        for (int iter = 0; iter < iterations; iter++) {
            vector<Point> P(n);
            for (int i = 0; i < n; i++) {
                P[i] = {dist_p(gen), dist_p(gen), dist_p(gen)};
            }

            auto start = chrono::high_resolution_clock::now();
            Ball b = welzl(P, true); // True = Shuffle
            auto end = chrono::high_resolution_clock::now();
            
            total_time += chrono::duration<double, milli>(end - start).count();
            total_boundary += b.boundary.size();
        }

        out1 << n << "," << (total_time / iterations) << "\n";
        out3 << n << "," << (total_boundary / iterations) << "\n";
        cout << "Done.\n";
    }
}

void run_worst_case_experiments() {
    vector<int> sizes = {10, 50, 100, 200, 300, 400, 500, 1000};
    int iterations = 5; // Fewer iterations because worst case takes a long time

    ofstream out2("data/exp2_worst_case.csv");
    out2 << "Size,AvgTimeShuffle_ms,AvgTimeNoShuffle_ms\n";

    mt19937 gen(42);
    uniform_real_distribution<double> dist_p(-1000.0, 1000.0);

    for (int n : sizes) {
        cout << "[Exp 2] Running Worst Case for N = " << n << "... " << flush;
        double total_time_shuffle = 0;
        double total_time_no_shuffle = 0;

        for (int iter = 0; iter < iterations; iter++) {
            vector<Point> P(n);
            for (int i = 0; i < n; i++) {
                P[i] = {dist_p(gen), dist_p(gen), dist_p(gen)};
            }
            
            // To create worst case, sort by distance to origin
            sort(P.begin(), P.end(), [](const Point& a, const Point& b) {
                return a.x*a.x + a.y*a.y + a.z*a.z < b.x*b.x + b.y*b.y + b.z*b.z;
            });

            vector<Point> P_copy1 = P;
            auto start1 = chrono::high_resolution_clock::now();
            welzl(P_copy1, true); // Shuffle enabled (Defeats the worst case)
            auto end1 = chrono::high_resolution_clock::now();
            total_time_shuffle += chrono::duration<double, milli>(end1 - start1).count();

            vector<Point> P_copy2 = P;
            auto start2 = chrono::high_resolution_clock::now();
            welzl(P_copy2, false); // No shuffle (Hits worst case O(N^4))
            auto end2 = chrono::high_resolution_clock::now();
            total_time_no_shuffle += chrono::duration<double, milli>(end2 - start2).count();
        }

        out2 << n << "," << (total_time_shuffle / iterations) << "," << (total_time_no_shuffle / iterations) << "\n";
        cout << "Done.\n";
    }
}

void generate_cube(int n, vector<Point>& P, mt19937& gen) {
    uniform_real_distribution<double> dist(-1000.0, 1000.0);
    for (int i = 0; i < n; i++) P[i] = {dist(gen), dist(gen), dist(gen)};
}

void generate_sphere(int n, vector<Point>& P, mt19937& gen) {
    uniform_real_distribution<double> dist(-1.0, 1.0);
    for (int i = 0; i < n; ) {
        double x = dist(gen) * 1000.0;
        double y = dist(gen) * 1000.0;
        double z = dist(gen) * 1000.0;
        if (x*x + y*y + z*z <= 1000.0*1000.0) {
            P[i++] = {x, y, z};
        }
    }
}

void generate_ellipsoid(int n, vector<Point>& P, mt19937& gen) {
    generate_sphere(n, P, gen);
    for (int i = 0; i < n; i++) {
        P[i].x *= 10.0; // Elongate
        P[i].y *= 0.5;
        P[i].z *= 0.5;
    }
}

void generate_clustered(int n, vector<Point>& P, mt19937& gen) {
    int num_clusters = 5;
    vector<Point> centers(num_clusters);
    uniform_real_distribution<double> dist_center(-1000.0, 1000.0);
    for (int i = 0; i < num_clusters; i++) {
        centers[i] = {dist_center(gen), dist_center(gen), dist_center(gen)};
    }
    normal_distribution<double> dist_noise(0.0, 20.0);
    uniform_int_distribution<int> dist_c(0, num_clusters - 1);
    for (int i = 0; i < n; i++) {
        int c = dist_c(gen);
        P[i] = {centers[c].x + dist_noise(gen), centers[c].y + dist_noise(gen), centers[c].z + dist_noise(gen)};
    }
}

void run_distribution_experiments() {
    vector<int> sizes = {1000, 10000, 50000, 100000, 200000, 500000, 1000000};
    int iterations = 20;

    ofstream out_time("data/exp4_dist_time.csv");
    out_time << "Size,Cube,Sphere,Ellipsoid,Clustered\n";

    ofstream out_boundary("data/exp5_dist_boundary.csv");
    out_boundary << "Size,Cube,Sphere,Ellipsoid,Clustered\n";

    mt19937 gen(1337);

    for (int n : sizes) {
        cout << "[Exp 4 & 5] Distributions for N = " << n << "... " << flush;
        
        vector<double> total_time(4, 0.0);
        vector<double> total_bound(4, 0.0);
        
        for (int iter = 0; iter < iterations; iter++) {
            vector<Point> P(n);
            
            // 0: Cube
            generate_cube(n, P, gen);
            auto start = chrono::high_resolution_clock::now();
            Ball b = welzl(P, true);
            total_time[0] += chrono::duration<double, milli>(chrono::high_resolution_clock::now() - start).count();
            total_bound[0] += b.boundary.size();
            
            // 1: Sphere
            generate_sphere(n, P, gen);
            start = chrono::high_resolution_clock::now();
            b = welzl(P, true);
            total_time[1] += chrono::duration<double, milli>(chrono::high_resolution_clock::now() - start).count();
            total_bound[1] += b.boundary.size();

            // 2: Ellipsoid
            generate_ellipsoid(n, P, gen);
            start = chrono::high_resolution_clock::now();
            b = welzl(P, true);
            total_time[2] += chrono::duration<double, milli>(chrono::high_resolution_clock::now() - start).count();
            total_bound[2] += b.boundary.size();

            // 3: Clustered
            generate_clustered(n, P, gen);
            start = chrono::high_resolution_clock::now();
            b = welzl(P, true);
            total_time[3] += chrono::duration<double, milli>(chrono::high_resolution_clock::now() - start).count();
            total_bound[3] += b.boundary.size();
        }

        out_time << n;
        for(int i=0; i<4; i++) out_time << "," << (total_time[i]/iterations);
        out_time << "\n";
        
        out_boundary << n;
        for(int i=0; i<4; i++) out_boundary << "," << (total_bound[i]/iterations);
        out_boundary << "\n";
        
        cout << "Done.\n";
    }
}

int main() {
    cout << "Starting Experiments...\n";
    run_random_experiments();
    run_worst_case_experiments();
    run_distribution_experiments();
    cout << "Experiments complete. CSV files generated in data/.\n";
    return 0;
}
