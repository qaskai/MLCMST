#pragma once

#include <network/mlcst.hpp>
#include <serializer.hpp>
#include <deserializer.hpp>

namespace MLCMST::network::serialization {

class MLCSTSerializer final : public Serializer<MLCST>
{
public:
    ~MLCSTSerializer() override;

    void serialize(const MLCST &mlcmst, std::ostream &stream) override;
};

class MLCMSTDeserializer final : public Deserializer<MLCST>
{
public:
    ~MLCMSTDeserializer() override;

    MLCST deserialize(std::istream &stream) override;
};

}