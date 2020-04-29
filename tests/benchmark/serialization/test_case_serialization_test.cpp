#include <catch2/catch.hpp>

#include <string>
#include <sstream>

#include <benchmark/serialization/test_case_serialization.hpp>

#include <util/util.hpp>

using namespace MLCMST;
using namespace MLCMST::benchmark;
using namespace MLCMST::benchmark::serialization;

TEST_CASE( "Test case serialization", "[benchmark][serialization][test_case]")
{
    TestCase test_case(3.5, network::MLCCNetwork(0, std::vector<network::CapacitatedNetwork> {
        network::CapacitatedNetwork(1, network::Network({
            { 0, 1.5, 2.5 },
            { 1.5, 0, 3 },
            { 2.5, 3, 0 }
        }))},
        { 0, 1, 2 }
    ));
    std::string expected_serialized_test_case =
        "3.5\n"
        "3 0 1\n"
        "0 1 2 \n"
        "1\n"
        "0 1.5 2.5 \n"
        "1.5 0 3 \n"
        "2.5 3 0 \n";
    std::stringstream ss;

    SECTION( "serialize" ) {
        TestCaseSerializer serializer;

        serializer.serialize(test_case, ss);
        std::string serialized_test_case = util::read_stream(ss);

        REQUIRE( serialized_test_case == expected_serialized_test_case );
    }
    SECTION( "deserialize" ) {
        TestCaseDeserializer deserializer;
        ss << expected_serialized_test_case;

        TestCase deserialized_test_case = deserializer.deserialize(ss);

        REQUIRE( deserialized_test_case.lowerBound() == test_case.lowerBound() );
        REQUIRE( deserialized_test_case.mlccNetwork() == test_case.mlccNetwork() );
    }
}
