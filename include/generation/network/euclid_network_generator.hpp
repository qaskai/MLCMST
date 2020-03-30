#pragma once

#include <memory>
#include <vector>

#include <model/network/network.hpp>
#include <model/geometry/point.hpp>
#include <generation/generator.hpp>

namespace MLCMST {
namespace generation {
namespace network {

using model::network::Network;
using model::geometry::Point;

using std::shared_ptr;
using std::vector;

class EuclidNetworkGenerator : public Generator<Network>
{

private:
    int N;
    shared_ptr<Generator<Point<double>>> point_generator;

    double eulcidDistance(const Point<double>& p, const Point<double>& q);
    vector<Point<double>> generatePoints(int N);
    vector<vector<double>> createDistanceMatrix(const vector<Point<double>>& points);

public:
    EuclidNetworkGenerator(int N);
    EuclidNetworkGenerator(int N, shared_ptr<Generator<Point<double>>> point_generator);
    ~EuclidNetworkGenerator();
    
    Network generate();
    
};

}
}
}
