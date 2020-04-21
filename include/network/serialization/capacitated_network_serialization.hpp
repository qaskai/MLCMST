#pragma once

#include <memory>

#include <network/capacitated_network.hpp>
#include <serializer.hpp>
#include <deserializer.hpp>


namespace MLCMST::network::serialization {


class CapacitatedNetworkSerializer final : public Serializer<CapacitatedNetwork>
{
public:
    CapacitatedNetworkSerializer();
    ~CapacitatedNetworkSerializer() override;
    
    void serialize(const CapacitatedNetwork& network, std::ostream& stream) override;

};



class CapacitatedNetworkDeserializer final : public Deserializer<CapacitatedNetwork>
{
public:
    CapacitatedNetworkDeserializer();
    ~CapacitatedNetworkDeserializer() override;
    
    CapacitatedNetwork deserialize(std::istream& stream) override;

};


}
