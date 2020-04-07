#pragma once

#include <network/network.hpp>
#include <serializer.hpp>
#include <deserializer.hpp>


namespace MLCMST {
namespace network {
namespace serialization {

class NetworkSerializer final : public Serializer<Network>
{
public:
    NetworkSerializer();
    ~NetworkSerializer();
    
    void serialize(const Network& network, std::ostream& stream);

};



class NetworkDeserializer final : public Deserializer<Network>
{
public:
    NetworkDeserializer();
    ~NetworkDeserializer();
    
    Network deserialize(std::istream& stream);
    
};

}
}
}
