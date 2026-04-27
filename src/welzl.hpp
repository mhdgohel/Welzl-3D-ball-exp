#pragma once

#include <vector>

struct Point {
    double x, y, z;
};

struct Ball {
    Point c;
    double r;
    std::vector<Point> boundary;

    bool contains(const Point& p) const;
};

// Computes the minimum enclosing ball of a set of 3D points.
// do_shuffle: If true, shuffles points randomly to guarantee O(N) expected time.
//             If false, processes in the given order (can degrade to O(N^4)).
Ball welzl(std::vector<Point>& P, bool do_shuffle = true);
