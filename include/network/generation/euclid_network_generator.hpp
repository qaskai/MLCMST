#pragma once

#include <memory>
#include <vector>

#include <network/network.hpp>
#include <geometry/point.hpp>
#include <generator.hpp>

namespace MLCMST::network::generation {

using geometry::Point;

using std::vector;

class EuclidNetworkGenerator final : public Generator<Network>
{
public:
    EuclidNetworkGenerator(int N, std::unique_ptr< Generator<Point> > point_generator);
    ~EuclidNetworkGenerator() override;

    Network generate() override;

private:
    std::unique_ptr< Generator<vector<Point>> > _point_set_generator;

    EuclidNetworkGenerator(std::unique_ptr< Generator<vector<Point>> > point_set_generator);

};

}
