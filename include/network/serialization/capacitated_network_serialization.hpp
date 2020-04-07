#pragma once

#include <memory>

#include <network/capacitated_network.hpp>
#include <util/serializer.hpp>
#include <util/deserializer.hpp>


namespace MLCMST {
namespace network {
namespace serialization {


class CapacitatedNetworkSerializer final : public util::Serializer<CapacitatedNetwork>
{
private:
    std::shared_ptr< util::Serializer<Network> > network_serializer;

public:
    CapacitatedNetworkSerializer();
    CapacitatedNetworkSerializer(std::shared_ptr< util::Serializer<Network> > network_serializer);
    ~CapacitatedNetworkSerializer();
    
    void serialize(const CapacitatedNetwork& network, std::ostream& stream);

};



class CapacitatedNetworkDeserializer final : public util::Deserializer<CapacitatedNetwork>
{
    std::shared_ptr< util::Deserializer<Network> > network_deserializer;

public:
    CapacitatedNetworkDeserializer();
    CapacitatedNetworkDeserializer(std::shared_ptr< util::Deserializer<Network> > network_deserializer);
    ~CapacitatedNetworkDeserializer();
    
    CapacitatedNetwork deserialize(std::istream& stream);
    
};


}
}
}
