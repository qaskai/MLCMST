#include <network/generation/euclid_mlcc_network_generator.hpp>

#include <utility>

#include <geometry/generation/real_point_generator.hpp>
#include <geometry/generation/real_point_set_generator.hpp>
#include <geometry/util.hpp>
#include <network/capacitated_network.hpp>
#include <util/util.hpp>

namespace MLCMST::network::generation {

EuclidMLCCNetworkGenerator::EuclidMLCCNetworkGenerator(
    int N, double from, double to, CenterPosition center_position, const std::vector<Level>& levels
)
    : EuclidMLCCNetworkGenerator(
        N, center_position, levels, std::make_unique< geometry::generation::RealPointGenerator >(from, to)
    )
{

}

EuclidMLCCNetworkGenerator::EuclidMLCCNetworkGenerator(
    int N, CenterPosition center_position, const std::vector<Level>& levels,
    std::unique_ptr< Generator<Point<double>> > point_generator
)
    : EuclidMLCCNetworkGenerator(
        center_position, levels,
        std::make_unique<geometry::generation::RealPointSetGenerator>(N, std::move(point_generator))
    )
{

}

EuclidMLCCNetworkGenerator::EuclidMLCCNetworkGenerator(
    CenterPosition center_position,
    std::vector<Level> levels,
    std::unique_ptr< Generator<std::vector<Point<double>>> > point_set_generator
)
    : _center_position(center_position), _levels(std::move(levels)), _point_set_generator(std::move(point_set_generator))
{

}

EuclidMLCCNetworkGenerator::~EuclidMLCCNetworkGenerator() = default;

MLCCNetwork EuclidMLCCNetworkGenerator::generate()
{
    vector<Point<double>> points = _point_set_generator->generate();
    Network network = Network(geometry::util::createDistanceMatrix(points));
    int N = network.vertexCount();
    vector<CapacitatedNetwork> network_levels;
    for (Level level : _levels) {
        auto new_costs = util::break_up(N, multiply(flatten(network.costs()), level.cost_multiplier));
        network_levels.emplace_back(level.capacity, Network(new_costs));
    }
    const int center = 0;
    vector<int> demands = vector<int>(N, 1);
    demands[center] = 0;
    return MLCCNetwork(center, network_levels, demands);
}

vector<double> EuclidMLCCNetworkGenerator::flatten(const vector<vector<double>>& v)
{
    vector<double> result;
    for (const vector<double>& inV : v) {
        for (double x : inV) {
            result.push_back(x);
        }
    }
    return result;
}

vector<double> EuclidMLCCNetworkGenerator::multiply(vector<double> v, double scalar)
{
    for (double& x : v) {
        x *= scalar;
    }
    return v;
}

}
