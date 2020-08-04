#include <catch2/catch.hpp>

#include <network/mlcmst.hpp>
#include <network/mlcc_network.hpp>

using namespace MLCMST;

TEST_CASE( "setMinimalViableLevels", "[network]" )
{
    using network::MLCCNetwork;
    using network::CapacitatedNetwork;
    MLCCNetwork mlcc_network(
            0,
            std::vector<CapacitatedNetwork>{
                CapacitatedNetwork(1, {}),
                CapacitatedNetwork(3, {}),
                CapacitatedNetwork(5, {}),
                },
                {0, 3, 1, 1}
    );
    std::vector<int> parents { 0, 0, 1, 2 };
    network::MLCMST mlcmst(parents.size(), 0);
    for (int i=0; i<parents.size(); i++) {
        mlcmst.parent(i) = parents[i];
    }

    bool valid_tree = mlcmst.setMinimalViableLevels(mlcc_network);

    REQUIRE( valid_tree );
    std::vector<int> expected_levels = { 0, 2, 1, 0 };
    REQUIRE( expected_levels == mlcmst.edgeLevels() );
}
