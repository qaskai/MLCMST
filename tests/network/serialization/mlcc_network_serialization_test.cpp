#include <catch2/catch.hpp>

#include <string>
#include <sstream>
#include <vector>

#include <network/mlcc_network.hpp>
#include <network/serialization/mlcc_network_serialization.hpp>
#include <util/util.hpp>

using namespace MLCMST;
using namespace MLCMST::network;


TEST_CASE( "Multi-level capacitated network serialization", "[network][serialization]" )
{
    MLCCNetwork network(
        1,
        {
            CapacitatedNetwork(1, Network({
                {0.0, 0.5, 0.7},
                {0.5, 0.0, 0.3},
                {0.7, 0.3, 0.0}
            })),
            CapacitatedNetwork(2, Network({
                {0.0, 0.8, 1.2},
                {0.8, 0.0, 0.5},
                {1.2, 0.5, 0.0}
            }))
        },
        { 1,0,2 }
    );
    std::string serialized_mlcc_network =
        "3 1 2\n"
        "1 0 2 \n"
        "1\n"
        "0 0.5 0.7 \n"
        "0.5 0 0.3 \n"
        "0.7 0.3 0 \n"
        "2\n"
        "0 0.8 1.2 \n"
        "0.8 0 0.5 \n"
        "1.2 0.5 0 \n";
    std::stringstream ss;

    SECTION( "serialize" ) {
        serialization::MLCCNetworkSerializer().serialize(network, ss);
        std::string result = util::read_stream(ss);

        REQUIRE( result == serialized_mlcc_network );
    }
    SECTION( "deserialize" ) {
        ss << serialized_mlcc_network;
        MLCCNetwork deserialized_network = serialization::MLCCNetworkDeserializer().deserialize(ss);

        REQUIRE(deserialized_network.vertexCount() == network.vertexCount() );
        REQUIRE(deserialized_network.levelsNumber() == network.levelsNumber() );
        for (int i=0; i< network.vertexCount(); i++) {
            REQUIRE(deserialized_network.demand(i) == network.demand(i) );
        }
        for (int i=0; i< deserialized_network.levelsNumber(); i++) {
            REQUIRE(deserialized_network.network(i).vertexCount() == network.vertexCount() );
            REQUIRE( deserialized_network.edgeCapacity(i) == network.edgeCapacity(i) );
            REQUIRE( deserialized_network.network(i) == network.network(i) );
        }
    }
}
