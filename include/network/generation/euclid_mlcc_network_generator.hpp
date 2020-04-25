#pragma once

#include <memory>
#include <vector>

#include <geometry/point.hpp>
#include <network/network.hpp>
#include <network/mlcc_network.hpp>
#include <generator.hpp>

namespace MLCMST::network::generation {

using geometry::Point;

using std::vector;

class EuclidMLCCNetworkGenerator final : public Generator<MLCCNetwork>
{
public:
    struct Level
    {
        int capacity;
        double cost_multiplier;
    };
    enum CenterPosition
    {
        RANDOM
    };

    EuclidMLCCNetworkGenerator(
            int N, double from, double to, CenterPosition center_position, const std::vector<Level>& levels);
    EuclidMLCCNetworkGenerator(
            int N, CenterPosition center_position, const std::vector<Level>& levels,
            std::unique_ptr< Generator<Point> > point_generator
    );
    ~EuclidMLCCNetworkGenerator() override;

    MLCCNetwork generate() override;

private:
    CenterPosition _center_position;
    std::vector<Level> _levels;
    std::unique_ptr< Generator<vector<Point>> > _point_set_generator;

    EuclidMLCCNetworkGenerator(
        CenterPosition center_position,
        std::vector<Level>  levels,
        std::unique_ptr< Generator<std::vector<Point>> > point_set_generator
    );

    vector<double> flatten(const vector<vector<double>>& v);
    vector<double> multiply(vector<double> v, double scalar);
};

}
