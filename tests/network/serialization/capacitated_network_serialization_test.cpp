#include <catch2/catch.hpp>

#include <string>
#include <sstream>

#include <network/network.hpp>
#include <network/capacitated_network.hpp>
#include <network/serialization/capacitated_network_serialization.hpp>
#include <util/util.hpp>

using namespace MLCMST;
using namespace MLCMST::network;

TEST_CASE( "Capacitated network serialization", "[network][serialization]" )
{
    CapacitatedNetwork network(5, Network({
        {0.0, 1.5, 2.5},
        {1.5, 0.0, 1.0},
        {2.5, 1.0, 0.0}
    }));

    std::string serialized_network =
        "3 5\n"
        "0 1.5 2.5 \n"
        "1.5 0 1 \n"
        "2.5 1 0 \n";
    std::stringstream ss;

    SECTION( "serialize" ) {
        serialization::CapacitatedNetworkSerializer().serialize(network, ss);
        std::string result = util::read_stream(ss);

        REQUIRE( result == serialized_network );
    }
    SECTION( "deserialize" ) {
        ss << serialized_network;
        CapacitatedNetwork deserialized_network =
            serialization::CapacitatedNetworkDeserializer().deserialize(ss);

        REQUIRE( deserialized_network.getEdgeCapacity() == network.getEdgeCapacity() );
        REQUIRE( deserialized_network.getNetwork() == network.getNetwork() );
    }
}