#pragma once


#include <network/mlcc_network.hpp>
#include <serializer.hpp>
#include <deserializer.hpp>


namespace MLCMST {
namespace network {
namespace serialization {


class MLCCNetworkSerializer final : Serializer<MLCCNetwork>
{

public:
    MLCCNetworkSerializer() = default;
    ~MLCCNetworkSerializer() = default;

    void serialize(const MLCCNetwork& network, std::ostream& stream);
};


class MLCCNetworkDeserializer final : Deserializer<MLCCNetwork>
{
public:
    MLCCNetworkDeserializer() = default;
    ~MLCCNetworkDeserializer() = default;

    MLCCNetwork deserialize(std::istream& stream);
};

}
}
}
