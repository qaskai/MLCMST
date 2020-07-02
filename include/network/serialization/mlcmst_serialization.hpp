#pragma once

#include <network/mlcmst.hpp>
#include <serializer.hpp>
#include <deserializer.hpp>

namespace MLCMST::network::serialization {

class MLCMSTSerializer final : public Serializer<MLCMST>
{
public:
    ~MLCMSTSerializer() override;

    void serialize(const MLCMST &mlcmst, std::ostream &stream) override;
};

class MLCMSTDeserializer final : public Deserializer<MLCMST>
{
public:
    ~MLCMSTDeserializer() override;

    MLCMST deserialize(std::istream &stream) override;
};

}