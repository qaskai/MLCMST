#include <catch2/catch.hpp>

#include <heuristic/genetic_gamvros.hpp>
#include <heuristic/link_upgrade.hpp>

#include <network/generation/euclid_mlcc_network_generator.hpp>

#include <geometry/generation/int_point_generator.hpp>

TEST_CASE( "Genetic Algorithm by Gamvros et. al.", "[heuristic]" )
{
    using MLCMST::heuristic::GeneticGamvros;
    using MLCMST::heuristic::LinkUpgrade;
    using MLCMST::network::generation::EuclidMLCCNetworkGenerator;
    using Level = MLCMST::network::generation::EuclidMLCCNetworkGenerator::Level;
    using MLCMST::network::MLCCNetwork;

    auto center_position = EuclidMLCCNetworkGenerator::CenterPosition::CENTER;
    auto demand_type = EuclidMLCCNetworkGenerator::DemandType::UNIT;
    auto levels = std::vector<Level>{
        Level{1,1.0}, Level{3, 2.0}, Level {10, 5.0} };
    auto network_generator = EuclidMLCCNetworkGenerator(
            20, center_position, demand_type, levels,
            std::make_unique<MLCMST::geometry::generation::IntPointGenerator>(0, 10, 1));

    MLCCNetwork mlcc_network = network_generator.generate();

    std::vector<std::unique_ptr<MLCMST::MLCMST_Solver> > init_solvers;
    init_solvers.emplace_back(std::make_unique<LinkUpgrade>(
            LinkUpgrade::Params{ true, false, false }));
    GeneticGamvros solver(std::move(init_solvers),
            std::make_unique<LinkUpgrade>(LinkUpgrade::Params{ true, false, false }),
            GeneticGamvros::Params{});

    auto result = solver.solve(mlcc_network);

    REQUIRE(result.mlcmst.has_value());
    auto mlcmst = result.mlcmst.value();
    REQUIRE(mlcmst.checkValidity(mlcc_network));
}
