#pragma once

#include <memory>
#include <vector>

#include <geometry/point.hpp>
#include <network/network.hpp>
#include <network/mlcc_network.hpp>
#include <generator.hpp>
#include <util/number/int_generator.hpp>

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
    enum class DemandType
    {
        UNIT, RANDOM, SET
    };
;
    EuclidMLCCNetworkGenerator(
        int N, CenterPosition center_position, DemandType demand_type, const std::vector<Level>& levels,
        std::unique_ptr< Generator<Point> > point_generator
    );
    ~EuclidMLCCNetworkGenerator() override;

    MLCCNetwork generate() override;

    void setDemands(const std::vector<int>& demands);

    [[nodiscard]] std::vector<Point> lastPointSet() const;

private:
    int _size;
    CenterPosition _center_position;
    DemandType _demand_type;
    std::vector<int> _demands;
    std::vector<Level> _levels;

    std::unique_ptr< Generator<vector<Point>> > _point_set_generator;
    util::number::IntGenerator _int_generator;

    std::vector<Point> _last_point_set;


    EuclidMLCCNetworkGenerator(
        int N,
        CenterPosition center_position,
        DemandType demand_type,
        std::vector<Level> levels,
        std::unique_ptr< Generator<std::vector<Point>> > point_set_generator
    );

    int determineCenter(const std::vector<Point>& points);
    std::vector<int> generateDemands(int max_capacity);

    vector<double> flatten(const vector<vector<double>>& v);
    vector<double> multiply(vector<double> v, double scalar);
};

}
