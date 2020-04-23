#include <catch2/catch.hpp>

#include <network/mlcc_network.hpp>
#include <network/mlcmst.hpp>
#include <solver/mp/escf.hpp>

using namespace MLCMST;
using namespace MLCMST::network;

TEST_CASE( "ESCF functional test", "[solver][mp][scf]" )
{
    MLCCNetwork mlcc_network(
        0,
        std::vector<CapacitatedNetwork>{
            CapacitatedNetwork(1, Network({
                { 0, 1, 2 },
                { 1, 0, 1 },
                { 2, 1, 0 }
            })),
            CapacitatedNetwork(2, Network({
                { 0, 1, 2 },
                { 1, 0, 2 },
                { 2, 2, 0 }
            })),
        },
        {0, 1, 1}
    );

    using ESCF = solver::mp::ESCF;
    SECTION( "exact solution" ) {
        ESCF scf(true);
        std::vector<int> expected_parents { 0, 0, 1 };
        std::vector<int> expected_levels { -1, 1, 0 };

        ESCF::Result result = scf.solve(mlcc_network);

        REQUIRE( result.finished );
        REQUIRE( result.lower_bound.value() == 2 );
        for (int i=0; i<mlcc_network.vertexCount(); i++) {
            if (i == mlcc_network.center())
                continue;
            REQUIRE( result.mlcmst->parent(i) == expected_parents[i] );
            REQUIRE( result.mlcmst->edgeLevel(i) == expected_levels[i] );
        }
    }
    SECTION( "linear programming" ) {
        ESCF scf;

        ESCF::Result result = scf.solve(mlcc_network);

        REQUIRE( result.finished );
        REQUIRE( ("this should give better results than SCF, but doesn't", result.lower_bound.value() == Approx(0).margin(0.0001)) );
        REQUIRE( !result.mlcmst.has_value() );
    }
}
