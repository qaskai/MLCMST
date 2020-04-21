#include <network/serialization/capacitated_network_serialization.hpp>

#include <network/serialization/fixed_size_network_serialization.hpp>

namespace MLCMST::network::serialization {

// ************ CapacitatedNetworkSerializer ************ //


CapacitatedNetworkSerializer::CapacitatedNetworkSerializer() = default;

CapacitatedNetworkSerializer::~CapacitatedNetworkSerializer() = default;

void CapacitatedNetworkSerializer::serialize(const CapacitatedNetwork& network, std::ostream& stream)
{
    stream << network.vertexCount() << " " << network.edgeCapacity() << "\n";
    FixedSizeNetworkSerializer(network.vertexCount()).serialize(network.network(), stream);
}


// ************ CapacitatedNetworkDeserializer ************ //

CapacitatedNetworkDeserializer::CapacitatedNetworkDeserializer() = default;

CapacitatedNetworkDeserializer::~CapacitatedNetworkDeserializer() = default;

CapacitatedNetwork CapacitatedNetworkDeserializer::deserialize(std::istream& stream)
{
    int size, edge_capacity;
    stream >> size >> edge_capacity;
    Network network = FixedSizeNetworkDeserializer(size).deserialize(stream);
    return CapacitatedNetwork(edge_capacity, network);
}


}
