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

    network::MLCMST run(const network::MLCCNetwork &mlcc_network) override;

private:
    std::unique_ptr< MLCMST_Heuristic > init_solver_;
    std::vector<std::function<network::MLCMST(const network::MLCMST&, const network::MLCCNetwork&)>>
    random_neighbour_generator_;

    Params params;

    util::number::IntGenerator int_generator_;
    std::default_random_engine random_engine_;

    static bool isUnitDemand(const network::MLCCNetwork& mlcc_network);

    network::MLCMST leafSwapNeighbourhood(const network::MLCMST& mlcmst, const network::MLCCNetwork& mlcc_network);
    network::MLCMST explodeSubtreeNeighbourhood(const network::MLCMST& mlcmst, const network::MLCCNetwork& mlcc_network);
    network::MLCMST gatherSingleNodesNeighbourhood(const network::MLCMST& mlcmst, const network::MLCCNetwork& mlcc_network);
    network::MLCMST levelDownRootSubtreeNeighbour(const network::MLCMST& mlcmst, const network::MLCCNetwork& mlcc_network);
    network::MLCMST mergeRootSubtreesNeighbour(const network::MLCMST& mlcmst, const network::MLCCNetwork& mlcc_network);

    static network::MLCMST localSearch(network::MLCMST mlcmst, const network::MLCCNetwork& mlcc_network);
};

}
