#include <network/generation/euclid_mlcc_network_generator.hpp>

#include <geometry/generation/real_point_set_generator.hpp>
#include <geometry/util.hpp>
#include <network/capacitated_network.hpp>
#include <network/generation/euclid_network_generator.hpp>


namespace MLCMST {
namespace network {
namespace generation {


EuclidMLCCNetworkGenerator::EuclidMLCCNetworkGenerator(
    int N, CenterPosition center_position, const std::vector<Level>& levels
) 
: EuclidMLCCNetworkGenerator(
    center_position, levels,
    std::make_unique< geometry::generation::RealPointSetGenerator >(N))
{

}

EuclidMLCCNetworkGenerator::EuclidMLCCNetworkGenerator(
    CenterPosition center_position,
    const std::vector<Level>& levels,
    std::unique_ptr< Generator<std::vector<Point<double>>> > point_set_generator
)
: center_position(center_position), levels(levels), point_set_generator(std::move(point_set_generator))
{

}

EuclidMLCCNetworkGenerator::~EuclidMLCCNetworkGenerator()
{
    
}

MLCCNetwork EuclidMLCCNetworkGenerator::generate()
{
    vector<Point<double>> points = point_set_generator->generate();
    Network network = Network(geometry::util::createDistanceMatrix(points));
    int N = network.getSize();
    vector<CapacitatedNetwork> network_levels;
    for (Level level : levels) {
        auto new_costs = break_up(N, multiply(flatten(network.getCosts()), level.cost_multiplier));
        network_levels.emplace_back(Network(new_costs), level.capacity);
    }
    vector<int> demands = vector<int>(N, 1);
    return MLCCNetwork(0, network_levels, demands);
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

vector<vector<double>> EuclidMLCCNetworkGenerator::break_up(int N, const vector<double>& v)
{
    vector<vector<double>> result;
    int idx = 0;
    while (idx < v.size()) {
        result.emplace_back();
        for (int i=0; i<N; i++) {
            if (idx >= v.size())
                break;
            result.back().push_back(v[idx++]);
        }
    }
    return result;
}

}
}
}
