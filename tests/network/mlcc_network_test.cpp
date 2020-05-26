#include <catch2/catch.hpp>

#include <network/mlcc_network.hpp>
#include <util/util.hpp>

TEST_CASE( "MLCCNetwork subNetwork", "[network]" )
{
    using MLCMST::network::CapacitatedNetwork;
    using MLCMST::network::MLCCNetwork;

    const double inf = MLCCNetwork::infinity();
    MLCCNetwork network(1, {
        CapacitatedNetwork(1, {
            { inf, 1, 2 },
            { 1, inf, 3 },
            { 2, 3, inf }
        }),
        CapacitatedNetwork(3, {
            { 0, 5, 6 },
            { 5, 0, inf },
            { 6, inf, 0 }
        })
    }, { 2, 0, 1 });

    auto [ sub_net, mapping ] = network.subNetwork({ 0 });
    auto rev_map = MLCMST::util::valueToIndex(mapping);

    REQUIRE( sub_net.center() == rev_map[network.center()] );
    for (int i=0; i<network.levelsNumber(); i++) {
        REQUIRE( sub_net.network(i) == network.network(i).subNetwork({ 0, 1 }).first );
    }
    for (int i=0; i<mapping.size(); i++) {
        REQUIRE( sub_net.demand(i) == network.demand(mapping[i]) );
    }
}
