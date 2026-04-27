#include <iostream>
#include <iomanip>
#include <chrono>
#include <random>
#include "welzl.hpp"

using namespace std;

int main() {
    cout << "Welzl's 3D Minimum Enclosing Ball Algorithm\n";
    cout << "-------------------------------------------\n";
    cout << "1. Generate random points\n";
    cout << "2. Enter points manually\n";
    cout << "Select option (1 or 2): ";
    int choice;
    if (!(cin >> choice)) return 0;
    
    vector<Point> points;
    if (choice == 1) {
        cout << "Enter number of points to generate (1 to 1,000,000): ";
        int n;
        cin >> n;
        if (n <= 0) n = 1;
        if (n > 1000000) n = 1000000;
        
        points.resize(n);
        mt19937 gen(1337);
        uniform_real_distribution<double> dist_p(-1000.0, 1000.0);
        
        for (int i = 0; i < n; i++) {
            points[i] = {dist_p(gen), dist_p(gen), dist_p(gen)};
        }
        cout << "Generated " << n << " points.\n";
    } else {
        cout << "Enter number of points: ";
        int n;
        cin >> n;
        points.resize(n);
        cout << "Enter " << n << " points (x y z):\n";
        for (int i = 0; i < n; i++) {
            cin >> points[i].x >> points[i].y >> points[i].z;
        }
    }
    
    auto start = chrono::high_resolution_clock::now();
    Ball b = welzl(points);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    
    cout << "\n--- Result ---\n";
    cout << fixed << setprecision(6);
    cout << "Center: (" << b.c.x << ", " << b.c.y << ", " << b.c.z << ")\n";
    cout << "Radius: " << b.r << "\n";
    cout << "Number of boundary points: " << b.boundary.size() << "\n";
    cout << "Boundary points:\n";
    for (size_t i = 0; i < b.boundary.size(); i++) {
        cout << "  " << i+1 << ": (" << b.boundary[i].x << ", " 
             << b.boundary[i].y << ", " << b.boundary[i].z << ")\n";
    }
    cout << "Execution time: " << duration.count() << " seconds\n";
    
    return 0;
}
