#pragma once

#include <cmath>
#include <vector>

#include <geometry/point.hpp>

namespace MLCMST::geometry::util {

template<typename T>
T euclidDistance(const Point<T>& p, const Point<T>& q);

std::vector<std::vector<double>> createDistanceMatrix(const std::vector<Point<double>>& points);



// *********** IMPLEMENTATION *********** //

template<typename T>
T euclidDistance(const Point<T>& p, const Point<T>& q) {
    T a = p.x - q.x;
    T b = p.y - q.y;
    return sqrt(a*a + b*b);
}

}