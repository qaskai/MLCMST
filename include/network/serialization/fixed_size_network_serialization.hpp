#pragma once

#include <network/network.hpp>
#include <serializer.hpp>
#include <deserializer.hpp>


namespace MLCMST::network::serialization {

class FixedSizeNetworkSerializer final : public Serializer<Network>
{
public:
    FixedSizeNetworkSerializer(int size);
    ~FixedSizeNetworkSerializer() override;

    void serialize(const Network& network, std::ostream& stream) override;

private:
    int _size;

};



class FixedSizeNetworkDeserializer final : public Deserializer<Network>
{
public:
    FixedSizeNetworkDeserializer(int size);
    ~FixedSizeNetworkDeserializer() override;

    Network deserialize(std::istream& stream) override;

private:
    int _size;

};

}
