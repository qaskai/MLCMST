#include <catch2/catch.hpp>

#include <string>
#include <sstream>

#include <network/network.hpp>
#include <network/serialization/fixed_size_network_serialization.hpp>
#include <util/util.hpp>

using namespace MLCMST;
using namespace MLCMST::network;


TEST_CASE( "Fixed size network serialization test" , "[network][serialization]" )
{
    const int size = 3;
    Network network({
        {0.0, 1.5, 2.5},
        {1.5, 0.0, 1.0},
        {2.5, 1.0, 0.0}
    });
    std::string serialized_network =
        "0 1.5 2.5 \n"
        "1.5 0 1 \n"
        "2.5 1 0 \n";
    std::stringstream ss;

    SECTION( "serialize" ) {
        serialization::FixedSizeNetworkSerializer(size).serialize(network, ss);
        std::string result = util::read_stream(ss);

        REQUIRE( result == serialized_network );
    }
    SECTION( "deserialize" ) {
        ss << serialized_network;
        Network deserialized_network = serialization::FixedSizeNetworkDeserializer(size).deserialize(ss);

        REQUIRE( deserialized_network == network );
    }
}