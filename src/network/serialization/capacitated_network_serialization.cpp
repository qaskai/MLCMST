#include <network/serialization/capacitated_network_serialization.hpp>

#include <network/serialization/network_serialization.hpp>

namespace MLCMST {
namespace network {
namespace serialization {

// ************ CapacitatedNetworkSerializer ************ //

CapacitatedNetworkSerializer::CapacitatedNetworkSerializer()
:   CapacitatedNetworkSerializer(std::make_shared<NetworkSerializer>())
{

}

CapacitatedNetworkSerializer::CapacitatedNetworkSerializer(
    std::shared_ptr< util::Serializer<Network> > network_serializer
)
:   network_serializer(network_serializer)
{

}

CapacitatedNetworkSerializer::~CapacitatedNetworkSerializer()
{

}


void CapacitatedNetworkSerializer::serialize(const CapacitatedNetwork& network, std::ostream& stream)
{
    stream << network.getEdgeCapacity() << std::endl;
    network_serializer->serialize(network.getNetwork(), stream);
}


// ************ CapacitatedNetworkDeserializer ************ //


CapacitatedNetworkDeserializer::CapacitatedNetworkDeserializer()
:   CapacitatedNetworkDeserializer(std::make_shared<NetworkDeserializer>())
{

}

CapacitatedNetworkDeserializer::CapacitatedNetworkDeserializer(
    std::shared_ptr< util::Deserializer<Network> > network_deserializer
)
:   network_deserializer(network_deserializer)
{

}

CapacitatedNetworkDeserializer::~CapacitatedNetworkDeserializer()
{

}


CapacitatedNetwork CapacitatedNetworkDeserializer::deserialize(std::istream& stream)
{
    int edge_capacity;
    stream >> edge_capacity;
    Network network = network_deserializer->deserialize(stream);
    return CapacitatedNetwork(network, edge_capacity);
}


}
}
}
