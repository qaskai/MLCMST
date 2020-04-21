#include <network/serialization/fixed_size_network_serialization.hpp>

#include <vector>

namespace MLCMST::network::serialization {


// *************** FixedSizeNetworkSerializer *************** //


FixedSizeNetworkSerializer::FixedSizeNetworkSerializer(int size) : _size(size)
{

}

FixedSizeNetworkSerializer::~FixedSizeNetworkSerializer() = default;

void FixedSizeNetworkSerializer::serialize(const Network& network, std::ostream& stream)
{
    for (int i=0; i < _size; i++) {
        for (int j=0; j < _size; j++) {
            stream << network.edgeCost(i, j) << " ";
        }
        stream << "\n";
    }
}


// *************** FixedSizeNetworkDeserializer *************** //

FixedSizeNetworkDeserializer::FixedSizeNetworkDeserializer(int size) : _size(size)
{

}

FixedSizeNetworkDeserializer::~FixedSizeNetworkDeserializer() = default;

Network FixedSizeNetworkDeserializer::deserialize(std::istream& stream)
{
    std::vector<std::vector<double>> costs(_size, std::vector<double>(_size));
    for (int i=0; i < _size; i++) {
        for (int j=0; j < _size; j++) {
            stream >> costs[i][j];
        }
    }
    return Network(costs);
}

}
