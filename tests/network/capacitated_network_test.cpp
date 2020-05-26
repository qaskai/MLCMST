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
