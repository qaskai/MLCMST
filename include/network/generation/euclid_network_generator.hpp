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

using std::vector;

class EuclidNetworkGenerator final : public Generator<Network>
{

private:
    std::unique_ptr< Generator<vector<Point<double>>> > point_set_generator;

    EuclidNetworkGenerator(std::unique_ptr< Generator<vector<Point<double>>> > point_set_generator);

public:
    EuclidNetworkGenerator(int N, double from = -10.0, double to = 10.0);
    EuclidNetworkGenerator(int N, std::unique_ptr< Generator<Point<double>> > point_generator);
    ~EuclidNetworkGenerator();
    
    Network generate();
    
};

}
}
}
