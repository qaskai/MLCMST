#include <catch2/catch.hpp>

#include <vector>

#include <network/generation/euclid_mlcc_network_generator.hpp>
#include <geometry/generation/int_point_generator.hpp>

#include <heuristic/improvement/local_search_2006.hpp>

TEST_CASE( "Gavmros local search 2006 heuristic random", "[heuristic][local_search]" )
{
    namespace generation = MLCMST::network::generation;
    using MLCMST::network::MLCCNetwork;
    using Level = generation::EuclidMLCCNetworkGenerator::Level;
    using CenterPos = generation::EuclidMLCCNetworkGenerator::CenterPosition;
    using DemandType = generation::EuclidMLCCNetworkGenerator::DemandType;

    std::vector<Level> levels{
            Level { 1, 1 },
            Level { 3, 2 },
            Level { 5, 3.5 }
    };
    generation::EuclidMLCCNetworkGenerator generator (30, CenterPos::RANDOM, DemandType::UNIT,
            levels, std::make_unique<MLCMST::geometry::generation::IntPointGenerator>(0 ,20));
    MLCCNetwork mlccNetwork = generator.generate();

    MLCMST::heuristic::improvement::LocalSearch2006 solver;
    auto result = solver.solve(mlccNetwork);

    REQUIRE( result.mlcst.has_value() );
    REQUIRE(result.mlcst->checkFeasibility(mlccNetwork) );

}