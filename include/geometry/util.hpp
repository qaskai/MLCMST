#pragma once

#include <cmath>
#include <vector>

#include <geometry/point.hpp>

namespace MLCMST::geometry::util {

double euclidDistance(const Point& p, const Point& q);
std::vector<std::vector<double>> createDistanceMatrix(const std::vector<Point>& points);

}
