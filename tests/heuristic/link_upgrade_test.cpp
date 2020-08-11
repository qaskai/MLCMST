#include <catch2/catch.hpp>

#include <network/mlcc_network.hpp>
#include <heuristic/improvement/link_upgrade.hpp>
#include <_util.hpp>

using namespace MLCMST;

TEST_CASE( "Link upgrade heuristic one step", "[heuristic][link_upgrade]" )
{
    using heuristic::improvement::LinkUpgrade;
    LinkUpgrade solver(LinkUpgrade::Params {
        .check_all_link_types = false, .reupgrade_nodes = false, .H_leafs_only=true
    });
    network::MLCCNetwork network(
        0,
        {
            network::CapacitatedNetwork(1, network::Network({
                {0, 2, 2, 2},
                {2, 0, 1, 1},
                {2, 1, 0, 1},
                {2, 1, 1, 0}
            })),
            network::CapacitatedNetwork(3, network::Network({
                {0, 3, 9, 9},
                {3, 0, 9, 9},
                {9, 9, 0, 9},
                {9, 9, 9, 0}
            }))
        },
        {0, 1, 1, 1});

    LinkUpgrade::Result result = solver.solve(network);
    network::MLCST mlcmst = result.mlcst.value();

    REQUIRE(mlcmst.checkFeasibility(network) );
    REQUIRE( mlcmst.parents() == std::vector<int>({ 0, 0, 1, 1 }) );
    REQUIRE(mlcmst.facilityLevels() == std::vector<int>({0, 1, 0, 0 }) );
}

TEST_CASE( "Link upgrade all variations functional", "[heuristic][link_upgrade]" )
{
    using heuristic::improvement::LinkUpgrade;

    network::MLCCNetwork mlcc_network = tests::Util::generateNetwork(30, false, 1);

    for (int i=0; i<8; i++) {
        LinkUpgrade::Params params{
            .check_all_link_types=((i&2)==0), .reupgrade_nodes=((i&4)==0), .H_leafs_only=((i & 1)==0)
        };
        LinkUpgrade linkUpgrade(params);

        auto result = linkUpgrade.solve(mlcc_network);

        REQUIRE( result.mlcst.has_value() );
        REQUIRE(result.mlcst.value().checkFeasibility(mlcc_network) );
    }
}
