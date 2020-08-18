#include <catch2/catch.hpp>

#include <vector>

#include <geometry/generation/int_point_generator.hpp>

#include <heuristic/improvement/link_upgrade.hpp>
#include <heuristic/improvement/local_search_2006.hpp>
#include <_util.hpp>

TEST_CASE( "Gavmros local search 2006 heuristic random", "[heuristic][local_search]" )
{
    using MLCMST::network::MLCCNetwork;
    using MLCMST::heuristic::improvement::LinkUpgrade;
    using MLCMST::heuristic::improvement::LocalSearch2006;

    MLCCNetwork mlccNetwork = MLCMST::tests::Util::generateNetwork(20, true, 1);

    MLCMST::heuristic::improvement::LocalSearch2006 solver(std::make_unique<LinkUpgrade>(LinkUpgrade::Params{
            false, false, true
    }), LocalSearch2006::Params { true });;
    auto result = solver.solve(mlccNetwork);

    REQUIRE( result.mlcst.has_value() );
    REQUIRE(result.mlcst->checkFeasibility(mlccNetwork) );
}
