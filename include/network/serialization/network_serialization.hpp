#pragma once

#include <network/network.hpp>
#include <serializer.hpp>
#include <deserializer.hpp>


namespace MLCMST::network::serialization {

class NetworkSerializer final : public Serializer<Network>
{
public:
    NetworkSerializer();
    ~NetworkSerializer() override;
    
    void serialize(const Network& network, std::ostream& stream) override;

};



class NetworkDeserializer final : public Deserializer<Network>
{
public:
    NetworkDeserializer();
    ~NetworkDeserializer() override;
    
    Network deserialize(std::istream& stream) override;
    
};

}
