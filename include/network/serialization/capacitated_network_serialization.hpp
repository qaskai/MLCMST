#pragma once

#include <memory>

#include <network/capacitated_network.hpp>
#include <serializer.hpp>
#include <deserializer.hpp>


namespace MLCMST {
namespace network {
namespace serialization {


class CapacitatedNetworkSerializer final : public Serializer<CapacitatedNetwork>
{

public:
    CapacitatedNetworkSerializer() = default;
    ~CapacitatedNetworkSerializer() = default;
    
    void serialize(const CapacitatedNetwork& network, std::ostream& stream);

};



class CapacitatedNetworkDeserializer final : public Deserializer<CapacitatedNetwork>
{
public:
    CapacitatedNetworkDeserializer() = default;
    ~CapacitatedNetworkDeserializer() = default;
    
    CapacitatedNetwork deserialize(std::istream& stream);
    
};


}
}
}
