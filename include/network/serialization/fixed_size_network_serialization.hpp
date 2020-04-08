#pragma once

#include <network/network.hpp>
#include <serializer.hpp>
#include <deserializer.hpp>


namespace MLCMST {
namespace network {
namespace serialization {

class FixedSizeNetworkSerializer final : public Serializer<Network>
{
private:
    int size;

public:
    FixedSizeNetworkSerializer(int size);
    ~FixedSizeNetworkSerializer() = default;
    
    void serialize(const Network& network, std::ostream& stream);

};



class FixedSizeNetworkDeserializer final : public Deserializer<Network>
{
private:
    int size;

public:
    FixedSizeNetworkDeserializer(int size);
    ~FixedSizeNetworkDeserializer() = default;
    
    Network deserialize(std::istream& stream);
    
};

}
}
}
