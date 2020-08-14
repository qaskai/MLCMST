#pragma once

#include <memory>
#include <vector>

#include <geometry/point.hpp>
#include <network/network.hpp>
#include <network/mlcc_network.hpp>
#include <generator.hpp>
#include <util/number/int_generator.hpp>
#include <util/util.hpp>

namespace MLCMST::network::generation {

using geometry::Point;

using std::vector;

class EuclidMLCCNetworkGenerator final : public Generator<MLCCNetwork>
{
public:
    struct Level
    {
        unsigned capacity;
        double cost_multiplier;
    };
    enum class CenterPosition
    {
        RANDOM, CORNER, CENTER
    };

    EuclidMLCCNetworkGenerator(
        int N, CenterPosition center_position, int max_demand, const std::vector<Level>& levels,
        std::unique_ptr< Generator<Point> > point_generator,
        long seed = util::clockMilliseconds()
    );
    ~EuclidMLCCNetworkGenerator() override;

    MLCCNetwork generate() override;

    [[nodiscard]] std::vector<Point> lastPointSet() const;

private:
    const int _size;
    const CenterPosition _center_position;
    const int _max_demand;
    const std::vector<Level> _levels;

    const std::unique_ptr< Generator<vector<Point>> > _point_set_generator;
    util::number::IntGenerator _random_int_generator;

    std::vector<Point> _last_point_set;


    EuclidMLCCNetworkGenerator(
        int N,
        CenterPosition center_position,
        int max_demand,
        std::vector<Level> levels,
        std::unique_ptr< Generator<std::vector<Point>> > point_set_generator,
        long seed
    );

    int determineCenter(const std::vector<Point>& points);
    std::vector<int> generateDemands();

    vector<double> flatten(const vector<vector<double>>& v);
    vector<double> multiply(vector<double> v, double scalar);
};

}
