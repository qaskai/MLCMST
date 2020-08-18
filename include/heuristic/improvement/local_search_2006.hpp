#pragma once

#include <memory>
#include <optional>
#include <tuple>
#include <unordered_map>

#include <mlcmst_solver.hpp>
#include <mlcmst_subnet_solver.hpp>
#include <heuristic/improvement/mlcmst_improver.hpp>

#include <network/network.hpp>
#include <network/mlcc_network.hpp>
#include <network/mlcst.hpp>

#include <json/property.hpp>

#include <util/number/int_generator.hpp>

namespace MLCMST::heuristic::improvement {

class LocalSearch2006 : public MLCMST_Improver
{
public:
    static std::string id();

    struct Params {
        bool cycle_search_iterate_all;

        constexpr static auto properties = std::make_tuple(
            json::Property<Params,bool>{&Params::cycle_search_iterate_all, "cycle_search_iterate_all"}
        );
    };

    LocalSearch2006(std::unique_ptr< MLCMST_Solver > subnet_solver, Params params);
    LocalSearch2006(std::unique_ptr< MLCMST_Solver > init_solver,
                    std::unique_ptr< MLCMST_Solver > subnet_solver, Params params);
    ~LocalSearch2006() override;

    network::MLCST improve(long steps, network::MLCST mlcmst, const network::MLCCNetwork &mlcc_network) override;
    std::vector<int> improvementStep(const network::MLCCNetwork& network, std::vector<int> group_ids);

private:
    static double EPS_;

    Params params_;

    MLCMST_SubnetSolver subnet_solver_;

    util::number::IntGenerator random_int_generator_;

    const network::MLCCNetwork* network_ = nullptr;

    /**
     * Graph structure :
     * 0 - N-1 --> regular nodes
     * N - 2N-1 --> pseudo nodes (valid only group_id + N, rest isolated vertices)
     * 2N --> origin node
     * @return Neighbourhood graph
     */
    network::Network buildNeighbourhoodGraph(const std::vector<int>& group_id);

    std::unordered_map<int, int> groupDemands(const std::vector<int>& group_id);
    std::unordered_map<int, std::vector<int>> groups(const std::vector<int>& group_id);
    std::optional<std::pair<std::vector<int>, double>> findBestProfitableExchange(int s, const network::Network& net,
            const std::vector<int>& group_id);

    std::optional<std::vector<int>> findBestProfitableExchange(const std::vector<int>& group_id);
    void implementExchange(std::vector<int> exchange, std::vector<int>& group_id);

};

}
