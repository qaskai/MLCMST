#include <catch2/catch.hpp>

#include <network/network.hpp>

TEST_CASE( "network neighbourhoods", "[network]" )
{
    using MLCMST::network::Network;

    const double inf = Network::infinity();
    Network network({
        { inf, 1, 5 },
        { 1, inf, inf },
        { 5, inf, inf }
    });

    SECTION( "neighbourhood list" ) {
        auto neighbourhoods = network.neighbourhoodList();
        REQUIRE( neighbourhoods[0] == std::vector<int>{ 1,2 } );
        REQUIRE( neighbourhoods[1] == std::vector<int>{ 0 } );
        REQUIRE( neighbourhoods[2] == std::vector<int>{ 0 } );
    }
    SECTION( "vertex neighbourhood" ) {
        auto neighbourhood0 = network.neighbourhood(0);
        REQUIRE( neighbourhood0 == std::vector<int>{ 1,2 } );
    }
}

TEST_CASE( "network subNet", "[network]" )
{
    using MLCMST::network::Network;

    const double inf = Network::infinity();
    const Network network({
        { inf, 1, 2, 3 },
        { 1, inf, 4, 5 },
        { 2, 4, inf, inf },
        { 3, 5, inf, inf }
    });

    auto [ subNet, mapping ] = network.subNetwork({ 1,2,3 });

    REQUIRE( mapping == std::vector<int>{ 1,2,3 });
    REQUIRE( subNet.costs() == std::vector<std::vector<double>>{
        { inf, 4, 5 },
        { 4, inf, inf },
        { 5, inf, inf }
    });

}
