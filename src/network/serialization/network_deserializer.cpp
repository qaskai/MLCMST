#include <network/serialization/network_deserializer.hpp>

#include <vector>

namespace MLCMST {
namespace network {
namespace serialization {

NetworkDeserializer::NetworkDeserializer()
{

}

NetworkDeserializer::~NetworkDeserializer()
{

}

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
}
}
