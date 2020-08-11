#include <catch2/catch.hpp>

#include <network/mlcc_network.hpp>
#include <network/mlcst.hpp>
#include <mp/mcf.hpp>

using namespace MLCMST;
using namespace MLCMST::network;

TEST_CASE( "MCF functional test", "[solver][mp][mcf]" ) {
    auto multiply_vector = [](double scalar, std::vector<double> v) {
        for (double &d : v) {
            d *= scalar;
        }
        return v;
    };
    std::vector<std::vector<double>> costs{
        {0,    4,    5, 5.83},
        {4,    0,    3, 3.16},
        {5,    3,    0, 1},
        {5.83, 3.16, 1, 0}
    };
    MLCCNetwork mlcc_network(
        0,
        std::vector<CapacitatedNetwork>{
            CapacitatedNetwork(1, Network(costs)),
            CapacitatedNetwork(3, Network({
                multiply_vector(2, costs[0]),
                multiply_vector(2, costs[1]),
                multiply_vector(2, costs[2]),
                multiply_vector(2, costs[3])
            })),
        },
{0, 1, 1, 1}
    );

    using MCF = mp::MCF;
    SECTION( "exact solution" ) {
        MCF mcf(true);
        std::vector<int> expected_parents { 0, 2, 0, 2 };
        std::vector<int> expected_levels { -1, 0, 1, 0 };

        MCF::Result result = mcf.solve(mlcc_network);

        REQUIRE( result.finished );
        REQUIRE( result.lower_bound.value() == Approx(14).margin(0.0001) );
        for (int i=0; i<mlcc_network.vertexCount(); i++) {
            if (i == mlcc_network.center())
                continue;
            REQUIRE(result.mlcst->parent(i) == expected_parents[i] );
            REQUIRE(result.mlcst->facilityLevel(i) == expected_levels[i] );
        }
    }
    SECTION( "linear programming" ) {
        MCF mcf;

        MCF::Result result = mcf.solve(mlcc_network);

        REQUIRE( result.finished );
        REQUIRE( result.lower_bound.value() == Approx(12.5).margin(0.0001) );
        REQUIRE( !result.mlcst.has_value() );
    }

}