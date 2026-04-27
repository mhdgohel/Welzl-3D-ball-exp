#include "welzl.hpp"
#include <cmath>
#include <random>
#include <algorithm>

const double EPS = 1e-7;

struct Vec {
    double x, y, z;
    Vec(double x, double y, double z) : x(x), y(y), z(z) {}
    Vec(Point p) : x(p.x), y(p.y), z(p.z) {}
    Vec operator-(const Vec& v) const { return Vec(x-v.x, y-v.y, z-v.z); }
    Vec operator+(const Vec& v) const { return Vec(x+v.x, y+v.y, z+v.z); }
    Vec operator*(double d) const { return Vec(x*d, y*d, z*d); }
    double dot(const Vec& v) const { return x*v.x + y*v.y + z*v.z; }
    Vec cross(const Vec& v) const { return Vec(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x); }
    double norm2() const { return x*x + y*y + z*z; }
    double norm() const { return std::sqrt(norm2()); }
};

Point toPoint(const Vec& v) { return {v.x, v.y, v.z}; }

double dist(const Point& a, const Point& b) {
    return (Vec(a) - Vec(b)).norm();
}

bool Ball::contains(const Point& p) const {
    return dist(c, p) <= r + EPS;
}

Ball ball_from_2(Point a, Point b) {
    Point c = { (a.x+b.x)/2.0, (a.y+b.y)/2.0, (a.z+b.z)/2.0 };
    return {c, dist(a, b)/2.0, {a, b}};
}

Ball ball_from_3(Point A, Point B, Point C) {
    Vec a = Vec(A) - Vec(C);
    Vec b = Vec(B) - Vec(C);
    Vec aXb = a.cross(b);
    double denom = 2.0 * aXb.norm2();
    if (denom <= 1e-10 * a.norm2() * b.norm2()) {
        double dAB = dist(A,B), dBC = dist(B,C), dCA = dist(C,A);
        if (dAB >= dBC && dAB >= dCA) return ball_from_2(A, B);
        if (dBC >= dAB && dBC >= dCA) return ball_from_2(B, C);
        return ball_from_2(C, A);
    }
    Vec center_offset = (aXb.cross(a) * b.norm2() + b.cross(aXb) * a.norm2()) * (1.0 / denom);
    Point center = toPoint(Vec(C) + center_offset);
    return {center, dist(center, A), {A, B, C}};
}

Ball ball_from_4(Point A, Point B, Point C, Point D) {
    Vec a = Vec(A) - Vec(D);
    Vec b = Vec(B) - Vec(D);
    Vec c = Vec(C) - Vec(D);
    
    double denom = 2.0 * a.dot(b.cross(c));
    if (std::abs(denom) <= 1e-10 * a.norm() * b.norm() * c.norm()) {
        Ball b1 = ball_from_3(A, B, C);
        Ball b2 = ball_from_3(A, B, D);
        Ball b3 = ball_from_3(A, C, D);
        Ball b4 = ball_from_3(B, C, D);
        Ball max_b = b1;
        if (b2.r > max_b.r) max_b = b2;
        if (b3.r > max_b.r) max_b = b3;
        if (b4.r > max_b.r) max_b = b4;
        return max_b;
    }
    
    Vec center_offset = (b.cross(c) * a.norm2() + c.cross(a) * b.norm2() + a.cross(b) * c.norm2()) * (1.0 / denom);
    Point center = toPoint(Vec(D) + center_offset);
    return {center, dist(center, A), {A, B, C, D}};
}

Ball welzl_with_3_boundary_points(const std::vector<Point>& P, int n, Point p1, Point p2, Point p3) {
    Ball b = ball_from_3(p1, p2, p3);
    for (int i = 0; i < n; i++) {
        if (!b.contains(P[i])) {
            b = ball_from_4(p1, p2, p3, P[i]);
        }
    }
    return b;
}

Ball welzl_with_2_boundary_points(const std::vector<Point>& P, int n, Point p1, Point p2) {
    Ball b = ball_from_2(p1, p2);
    for (int i = 0; i < n; i++) {
        if (!b.contains(P[i])) {
            b = welzl_with_3_boundary_points(P, i, p1, p2, P[i]);
        }
    }
    return b;
}

Ball welzl_with_1_boundary_point(const std::vector<Point>& P, int n, Point p1) {
    Ball b = {p1, 0.0, {p1}};
    for (int i = 0; i < n; i++) {
        if (!b.contains(P[i])) {
            b = welzl_with_2_boundary_points(P, i, p1, P[i]);
        }
    }
    return b;
}

Ball welzl(std::vector<Point>& P, bool do_shuffle) {
    if (P.empty()) return {{0,0,0}, 0.0, {}};
    
    if (do_shuffle) {
        std::mt19937 gen(42);
        std::shuffle(P.begin(), P.end(), gen);
    }
    
    Ball b = {P[0], 0.0, {P[0]}};
    for (int i = 1; i < P.size(); i++) {
        if (!b.contains(P[i])) {
            b = welzl_with_1_boundary_point(P, i, P[i]);
        }
    }
    return b;
}
