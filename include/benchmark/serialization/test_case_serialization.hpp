#pragma once

#include <memory>

#include <serializer.hpp>
#include <deserializer.hpp>
#include <benchmark/test_case.hpp>
#include <network/mlcc_network.hpp>

namespace MLCMST::benchmark::serialization {

class TestCaseSerializer : public Serializer<TestCase>
{
public:
    TestCaseSerializer();
    explicit TestCaseSerializer(std::unique_ptr< Serializer<network::MLCCNetwork> > mlcc_network_serializer);

    void serialize(const TestCase& testCase, std::ostream& out) override;

private:
    std::unique_ptr< Serializer<network::MLCCNetwork> > _mlcc_network_serializer;
};

class TestCaseDeserializer : public Deserializer<TestCase>
{
public:
    TestCaseDeserializer();
    explicit TestCaseDeserializer(std::unique_ptr< Deserializer<network::MLCCNetwork> > mlcc_network_deserializer);

    TestCase deserialize(std::istream& in) override;

private:
    std::unique_ptr< Deserializer<network::MLCCNetwork> > _mlcc_network_deserializer;
};

}
