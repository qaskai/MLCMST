#include <benchmark/serialization/test_case_serialization.hpp>

#include <network/serialization/mlcc_network_serialization.hpp>

namespace MLCMST::benchmark::serialization {

// TestCaseSerializer

TestCaseSerializer::TestCaseSerializer()
    : TestCaseSerializer(std::make_unique<network::serialization::MLCCNetworkSerializer>())
{

}

TestCaseSerializer::TestCaseSerializer(std::unique_ptr<Serializer<network::MLCCNetwork>> mlcc_network_serializer)
    : _mlcc_network_serializer(std::move(mlcc_network_serializer))
{

}

void TestCaseSerializer::serialize(const TestCase &testCase, std::ostream &out)
{
    out << testCase.lowerBound() << "\n";
    _mlcc_network_serializer->serialize(testCase.mlccNetwork(), out);
}

// TestCaseDeserializer

TestCaseDeserializer::TestCaseDeserializer()
    : TestCaseDeserializer(std::make_unique<network::serialization::MLCCNetworkDeserializer>())
{

}

TestCaseDeserializer::TestCaseDeserializer(std::unique_ptr<Deserializer<network::MLCCNetwork>> mlcc_network_deserializer)
    : _mlcc_network_deserializer(std::move(mlcc_network_deserializer))
{

}

TestCase TestCaseDeserializer::deserialize(std::istream &in)
{
    double lower_bound;
    in >> lower_bound;
    network::MLCCNetwork mlcc_network = _mlcc_network_deserializer->deserialize(in);
    return TestCase(lower_bound, mlcc_network);
}

}
