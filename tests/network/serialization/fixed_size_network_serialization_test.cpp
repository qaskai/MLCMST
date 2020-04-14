#include <catch2/catch.hpp>

#include <string>
#include <sstream>

#include <network/network.hpp>
#include <network/serialization/fixed_size_network_serialization.hpp>
#include <util/util.hpp>

using namespace MLCMST;
using namespace MLCMST::network;


bool network_equal(const Network& n1, const Network& n2)
{
    if (n1.getSize() != n2.getSize())
        return false;
    int size = n1.getSize();
    for (int i=0; i<size; i++) {
        for (int j=0; j<size; j++) {
            if (n1.edgeCost(i,j) != n2.edgeCost(i,j))
                return false;
        }
    }

    return true;
}

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

        REQUIRE( network_equal( deserialized_network, network ) );
    }
}