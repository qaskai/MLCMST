#include <network/serialization/network_serialization.hpp>

#include <vector>

namespace MLCMST::network::serialization {


// *************** NetworkSerializer *************** //


NetworkSerializer::NetworkSerializer() = default;

NetworkSerializer::~NetworkSerializer() = default;

void NetworkSerializer::serialize(const Network& network, std::ostream& stream)
{
    int N = network.vertexCount();
    stream << N << "\n";
    for (int i=0; i<N; i++) {
        for (int j=0; j<N; j++) {
            stream << network.edgeCost(i, j) << " ";
        }
        stream << "\n";
    }
}




// *************** NetworkDeserializer *************** //

NetworkDeserializer::NetworkDeserializer() = default;

NetworkDeserializer::~NetworkDeserializer() = default;

Network NetworkDeserializer::deserialize(std::istream& stream)
{
    int N;
    stream >> N;
    std::vector<std::vector<double>> costs(N, std::vector<double>(N));
    for (int i=0; i<N; i++) {
        for (int j=0; j<N; j++) {
            stream >> costs[i][j];
        }
    }

    return Network(costs);
}

}