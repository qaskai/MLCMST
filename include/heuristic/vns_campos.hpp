#pragma once

#include <memory>
#include <functional>
#include <vector>
#include <tuple>
#include <string>
#include <random>

#include <network/mlcc_network.hpp>
#include <heuristic/mlcmst_heuristic.hpp>

#include <util/number/int_generator.hpp>

#include <json/property.hpp>

namespace MLCMST::heuristic {

class VNS_Campos : public MLCMST_Heuristic
{
public:
    struct Params {
        int max_failed_iterations;

        static constexpr auto properties = std::make_tuple(
            json::Property<Params, int>{&Params::max_failed_iterations, "max_failed_iterations"}
            );
    };

    static std::string id();

    VNS_Campos(std::unique_ptr< MLCMST_Heuristic > init_solver, Params params);
    ~VNS_Campos() override;

    network::MLCST run(const network::MLCCNetwork &mlcc_network) override;

private:
    std::unique_ptr< MLCMST_Heuristic > init_solver_;
    std::vector<std::function<network::MLCST(const network::MLCST&, const network::MLCCNetwork&)>>
    random_neighbour_generator_;

    Params params;

    util::number::IntGenerator int_generator_;
    std::default_random_engine random_engine_;

    static bool isUnitDemand(const network::MLCCNetwork& mlcc_network);

    network::MLCST leafSwapNeighbourhood(const network::MLCST& mlcmst, const network::MLCCNetwork& mlcc_network);
    network::MLCST explodeSubtreeNeighbourhood(const network::MLCST& mlcmst, const network::MLCCNetwork& mlcc_network);
    network::MLCST gatherSingleNodesNeighbourhood(const network::MLCST& mlcmst, const network::MLCCNetwork& mlcc_network);
    network::MLCST levelDownRootSubtreeNeighbour(const network::MLCST& mlcmst, const network::MLCCNetwork& mlcc_network);
    network::MLCST mergeRootSubtreesNeighbour(const network::MLCST& mlcmst, const network::MLCCNetwork& mlcc_network);

    static network::MLCST localSearch(network::MLCST mlcmst, const network::MLCCNetwork& mlcc_network);
};

}
