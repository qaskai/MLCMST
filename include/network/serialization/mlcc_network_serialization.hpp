#pragma once


#include <network/mlcc_network.hpp>
#include <serializer.hpp>
#include <deserializer.hpp>


namespace MLCMST {
namespace network {
namespace serialization {

/**
 * Format:
 * vertexCount center level_number
 * demands vector
 * capacity_level1
 * network_level1
 * capacity_level2
 * network_level2
 *
 **/

class MLCCNetworkSerializer final : public Serializer<MLCCNetwork>
{
public:
    MLCCNetworkSerializer();
    ~MLCCNetworkSerializer() override;

    void serialize(const MLCCNetwork& network, std::ostream& stream) override;
};


class MLCCNetworkDeserializer final : public Deserializer<MLCCNetwork>
{
public:
    MLCCNetworkDeserializer();
    ~MLCCNetworkDeserializer() override;

    MLCCNetwork deserialize(std::istream& stream) override;
};

}
}
}
