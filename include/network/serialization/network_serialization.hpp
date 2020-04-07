#pragma once

#include <network/network.hpp>
#include <util/serializer.hpp>
#include <util/deserializer.hpp>


namespace MLCMST {
namespace network {
namespace serialization {

class NetworkSerializer : public util::Serializer<Network>
{
public:
    NetworkSerializer();
    ~NetworkSerializer();
    
    void serialize(const Network& network, std::ostream& stream);

};



class NetworkDeserializer : public util::Deserializer<Network>
{
public:
    NetworkDeserializer();
    ~NetworkDeserializer();
    
    Network deserialize(std::istream& stream);
    
};

}
}
}
