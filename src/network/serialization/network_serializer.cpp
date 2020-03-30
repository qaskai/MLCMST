#include <network/serialization/network_serializer.hpp>

namespace MLCMST {
namespace network {
namespace serialization {

NetworkSerializer::NetworkSerializer()
{

}

NetworkSerializer::~NetworkSerializer()
{

}

void NetworkSerializer::serialize(const Network& network, std::ostream& stream)
{
    int N = network.getSize();
    stream << N << std::endl;
    for (int i=0; i<N; i++) {
        for (int j=0; j<N; j++) {
            stream << network.edgeCost(i, j) << " ";
        }
        stream << std::endl;
    }
}

}
}
}
