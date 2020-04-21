#include <iostream>
#include <vector>

#include <network/mlcc_network.hpp>
#include <network/generation/euclid_mlcc_network_generator.hpp>

#include <network/serialization/mlcc_network_serialization.hpp>

#include <solver/mp/scf.hpp>

using namespace std;

using namespace MLCMST;
using namespace MLCMST::network;

void scfRun(network::MLCCNetwork mlcc_network)
{
    solver::mp::SCF scf(true);
    auto result = scf.solve(mlcc_network);

    cout << result.lower_bound.value() << endl;
}

network::MLCCNetwork generateNetwork()
{
    using Level = generation::EuclidMLCCNetworkGenerator::Level;
    using CenterPos = generation::EuclidMLCCNetworkGenerator::CenterPosition;

    vector<Level> levels{
        Level { 1, 1 },
        Level { 3, 2 },
        Level { 5, 3.5 }
    };
    generation::EuclidMLCCNetworkGenerator generator (9, 0, 10, CenterPos::RANDOM, levels);
    MLCCNetwork mlccNetwork = generator.generate();

    mlccNetwork = generator.generate();

    serialization::MLCCNetworkSerializer serializer;
    serializer.serialize(mlccNetwork, cout);
    return mlccNetwork;
}

int main(int argc, char const *argv[])
{
    scfRun(generateNetwork());
    return 0;
}