#include <network/generation/euclid_ml_centralized_capacitated_network_generator.hpp>

#include <geometry/generation/real_point_set_generator.hpp>
#include <network/capacitated_network.hpp>
#include <network/generation/euclid_network_generator.hpp>


namespace MLCMST {
namespace network {
namespace generation {


EuclidMLCentralizedCapacitatedNetworkGenerator::EuclidMLCentralizedCapacitatedNetworkGenerator(
    int N, const std::vector<Level>& levels
    ) : EuclidMLCentralizedCapacitatedNetworkGenerator(
        levels, std::make_unique< geometry::generation::RealPointSetGenerator >(N))
{

}

EuclidMLCentralizedCapacitatedNetworkGenerator::EuclidMLCentralizedCapacitatedNetworkGenerator(
    const std::vector<Level>& levels,
    std::unique_ptr< Generator<std::vector<Point<double>>> > point_set_generator
    ) : levels(levels), network_generator( std::make_unique<EuclidNetworkGenerator>(std::move(point_set_generator)) )
{

}

EuclidMLCentralizedCapacitatedNetworkGenerator::~EuclidMLCentralizedCapacitatedNetworkGenerator()
{
    
}

MLCentralizedCapacitatedNetwork EuclidMLCentralizedCapacitatedNetworkGenerator::generate()
{
    Network network = network_generator->generate();
    int N = network.getSize();
    vector<CapacitatedNetwork> network_levels;
    for (Level level : levels) {
        auto new_costs = break_up(N, multiply(flatten(network.getCosts()), level.cost_multiplier));
        network_levels.emplace_back(new_costs, level.capacity);
    }
    vector<int> demands = vector<int>(N, 1);
    return MLCentralizedCapacitatedNetwork(0, network_levels, demands);
}

vector<double> EuclidMLCentralizedCapacitatedNetworkGenerator::flatten(const vector<vector<double>>& v)
{
    vector<double> result;
    for (const vector<double>& inV : v) {
        for (double x : inV) {
            result.push_back(x);
        }
    }
    return result;
}

vector<double> EuclidMLCentralizedCapacitatedNetworkGenerator::multiply(vector<double> v, double scalar)
{
    for (double& x : v) {
        x *= scalar;
    }
    return v;
}

vector<vector<double>> EuclidMLCentralizedCapacitatedNetworkGenerator::break_up(int N, const vector<double>& v)
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
