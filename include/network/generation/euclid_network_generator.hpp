#pragma once

#include <memory>
#include <vector>

#include <network/network.hpp>
#include <geometry/point.hpp>
#include <generator.hpp>

namespace MLCMST {
namespace network {
namespace generation {

using geometry::Point;

using std::unique_ptr;
using std::vector;

class EuclidNetworkGenerator : public Generator<Network>
{

private:
    int N;
    unique_ptr<Generator<Point<double>>> point_generator;

    double eulcidDistance(const Point<double>& p, const Point<double>& q);
    vector<Point<double>> generatePoints(int N);
    vector<vector<double>> createDistanceMatrix(const vector<Point<double>>& points);

public:
    EuclidNetworkGenerator(int N);
    EuclidNetworkGenerator(int N, unique_ptr<Generator<Point<double>>> point_generator);
    ~EuclidNetworkGenerator();
    
    Network generate();
    
};

}
}
}
