#include <iostream>
#include <vector>

#include <network/mlcc_network.hpp>
#include <network/generation/euclid_mlcc_network_generator.hpp>

#include <network/serialization/mlcc_network_serialization.hpp>

#include <mp/escf.hpp>

using namespace std;

using namespace MLCMST;
using namespace MLCMST::network;

void scfRun(network::MLCCNetwork mlcc_network)
{
    mp::SCF scf(false);
    auto result = scf.solve(mlcc_network);

    cout << result.lower_bound.value() << endl;
    cout << result.wall_time / 1000.0 << endl;
}

network::MLCCNetwork generateNetwork()
{
    using Level = generation::EuclidMLCCNetworkGenerator::Level;
    using CenterPos = generation::EuclidMLCCNetworkGenerator::CenterPosition;

    vector<Level> levels{
        Level { 1, 1 },
        Level { 3, 2 },
        Level { 10, 6 }
    };
    generation::EuclidMLCCNetworkGenerator generator (10, 0, 20, CenterPos::RANDOM, levels);
    MLCCNetwork mlccNetwork = generator.generate();

    mlccNetwork = generator.generate();

//    serialization::MLCCNetworkSerializer serializer;
//    serializer.serialize(mlccNetwork, cout);
    return mlccNetwork;
}

int main(int argc, char const *argv[])
{
    scfRun(generateNetwork());
    return 0;
}