#include <catch2/catch.hpp>

#include <network/capacitated_network.hpp>

TEST_CASE( "CapacitatedNetwork subNetwork", "[network]" )
{
    using MLCMST::network::CapacitatedNetwork;

    const double inf = CapacitatedNetwork::infinity();
    CapacitatedNetwork network(3, {
        { inf, 1, 2 },
        { 1, 0, inf },
        { 2, inf, inf }
    });

    auto [ sub_net, mapping ] = network.subNetwork({ 2,1 });

    REQUIRE( mapping == std::vector<int>{ 2,1 } );
    REQUIRE( sub_net.edgeCapacity() == network.edgeCapacity() );
    REQUIRE( sub_net.network().costs() == std::vector<std::vector<double>>{
        { inf, inf },
        { inf, 0 }
    });
}

TEST_CASE( "CapacitatedNetwork operators", "[network]" )
{
    using MLCMST::network::CapacitatedNetwork;

    CapacitatedNetwork network(3, {
        { 1.2, 2.3, 0.3 },
        { 0.4, 2.3, 1.3 },
        { 0.4, 0.0, 0.0 }
    });

    SECTION( "equality" ) {
        CapacitatedNetwork n = network;
        REQUIRE( n == network );
        REQUIRE( network == n );
    }
    SECTION( "inequality different costs" ) {
        CapacitatedNetwork n = network;
        n.edgeCost(0,0) = 0;
        REQUIRE( n != network );
        REQUIRE( network != n );
    }
    SECTION( "inequality different capacity" ) {
        CapacitatedNetwork n(1, network.network());
        REQUIRE( n != network );
        REQUIRE( network != n );
    }
    SECTION( "multiplication" ) {
        double scalar = 1.5;
        CapacitatedNetwork n1 = scalar * network;
        CapacitatedNetwork n2 = network * scalar;

        REQUIRE( n1 == n2 );
        REQUIRE( n1.edgeCapacity() == network.edgeCapacity() );
        REQUIRE( n1.network() == scalar * network.network() );

    }
}
