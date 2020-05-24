#include <catch2/catch.hpp>

#include <network/mlcc_network.hpp>
#include <heuristic/link_upgrade_ud.hpp>

using namespace MLCMST;

TEST_CASE( "Link upgrade heuristic one step", "[heuristic][link_upgrade]" )
{
    heuristic::LinkUpgradeUD solver;
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

    heuristic::LinkUpgradeUD::Result result = solver.solve(network);
    network::MLCMST mlcmst = result.mlcmst.value();

    REQUIRE(mlcmst.checkValidity(network) );
    REQUIRE( mlcmst.parents() == std::vector<int>({ 0, 0, 1, 1 }) );
    REQUIRE( mlcmst.edgeLevels() == std::vector<int>({ 0, 1, 0, 0 }) );
}
