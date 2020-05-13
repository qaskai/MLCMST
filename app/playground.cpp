#include <iostream>
#include <vector>

#include <geometry/generation/int_point_generator.hpp>

#include <network/mlcc_network.hpp>
#include <network/generation/euclid_mlcc_network_generator.hpp>

#include <network/serialization/mlcc_network_serialization.hpp>

#include <heuristic/link_upgrade_unit_demand.hpp>

using namespace std;

using namespace MLCMST;
using namespace MLCMST::network;


network::MLCCNetwork generateNetwork()
{
    using Level = generation::EuclidMLCCNetworkGenerator::Level;
    using CenterPos = generation::EuclidMLCCNetworkGenerator::CenterPosition;

    vector<Level> levels{
            Level { 1, 1 },
            Level { 3, 2 },
            Level { 10, 6 }
    };
    generation::EuclidMLCCNetworkGenerator generator (10, CenterPos::RANDOM, levels,
            std::make_unique<geometry::generation::IntPointGenerator>(0 ,20));
    MLCCNetwork mlccNetwork = generator.generate();

    mlccNetwork = generator.generate();

//    serialization::MLCCNetworkSerializer serializer;
//    serializer.serialize(mlccNetwork, cout);
    return mlccNetwork;
}

void run(const MLCCNetwork& network)
{
    heuristic::LinkUpgradeUnitDemand solver;
    auto result = solver.solve(network);
    std::cout << result.mlcmst.value().cost(network) << std::endl;
}

int main(int argc, char const *argv[])
{
    run(generateNetwork());
    return 0;
}