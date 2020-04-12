#include <network/serialization/fixed_size_network_serialization.hpp>

#include <vector>

namespace MLCMST {
namespace network {
namespace serialization {


// *************** FixedSizeNetworkSerializer *************** //


FixedSizeNetworkSerializer::FixedSizeNetworkSerializer(int size) : size(size)
{

}

void FixedSizeNetworkSerializer::serialize(const Network& network, std::ostream& stream)
{
    for (int i=0; i<size; i++) {
        for (int j=0; j<size; j++) {
            stream << network.edgeCost(i, j) << " ";
        }
        stream << "\n";
    }
}



// *************** FixedSizeNetworkDeserializer *************** //

FixedSizeNetworkDeserializer::FixedSizeNetworkDeserializer(int size) : size(size)
{

}

Network FixedSizeNetworkDeserializer::deserialize(std::istream& stream)
{
    std::vector<std::vector<double>> costs(size, std::vector<double>(size));
    for (int i=0; i<size; i++) {
        for (int j=0; j<size; j++) {
            stream >> costs[i][j];
        }
    }
    return Network(costs);
}

}
}
}
