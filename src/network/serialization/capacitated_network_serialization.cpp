#include <network/serialization/capacitated_network_serialization.hpp>

#include <network/serialization/fixed_size_network_serialization.hpp>

namespace MLCMST {
namespace network {
namespace serialization {

// ************ CapacitatedNetworkSerializer ************ //


void CapacitatedNetworkSerializer::serialize(const CapacitatedNetwork& network, std::ostream& stream)
{
    stream << network.getSize() << " " <<  network.getEdgeCapacity() << std::endl;
    FixedSizeNetworkSerializer(network.getSize()).serialize(network.getNetwork(), stream);
}


// ************ CapacitatedNetworkDeserializer ************ //

CapacitatedNetwork CapacitatedNetworkDeserializer::deserialize(std::istream& stream)
{
    int size, edge_capacity;
    stream >> size >> edge_capacity;
    Network network = FixedSizeNetworkDeserializer(size).deserialize(stream);
    return CapacitatedNetwork(network, edge_capacity);
}


}
}
}
