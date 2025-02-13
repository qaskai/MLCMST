#include <catch2/catch.hpp>

#include <heuristic/genetic_gamvros.hpp>
#include <heuristic/improvement/link_upgrade.hpp>

#include <network/generation/euclid_mlcc_network_generator.hpp>

#include <geometry/generation/int_point_generator.hpp>

TEST_CASE( "Genetic Algorithm by Gamvros et. al.", "[heuristic]" )
{
    using MLCMST::heuristic::GeneticGamvros;
    using MLCMST::heuristic::improvement::LinkUpgrade;
    using MLCMST::network::generation::EuclidMLCCNetworkGenerator;
    using Level = MLCMST::network::generation::EuclidMLCCNetworkGenerator::Level;
    using MLCMST::network::MLCCNetwork;

    auto center_position = EuclidMLCCNetworkGenerator::CenterPosition::CENTER;
    const int max_demand = 1;
    auto levels = std::vector<Level>{
        Level{1,1.0}, Level{3, 2.0}, Level {10, 5.0} };
    auto network_generator = EuclidMLCCNetworkGenerator(
            20, center_position, max_demand, levels,
            std::make_unique<MLCMST::geometry::generation::IntPointGenerator>(0, 10, 1));

    MLCCNetwork mlcc_network = network_generator.generate();

    std::vector<std::unique_ptr<MLCMST::MLCMST_Solver> > init_solvers;
    init_solvers.emplace_back(std::make_unique<LinkUpgrade>(
            LinkUpgrade::Params{ true, false, false }));
    GeneticGamvros solver(std::move(init_solvers),
            std::make_unique<LinkUpgrade>(LinkUpgrade::Params{ true, false, false }),
            GeneticGamvros::Params{});

    auto result = solver.solve(mlcc_network);

    REQUIRE(result.mlcst.has_value());
    auto mlcmst = result.mlcst.value();
    REQUIRE(mlcmst.checkFeasibility(mlcc_network));
}
