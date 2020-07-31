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
#include <network/mlcmst.hpp>

namespace MLCMST::heuristic::improvement {

class LocalSearch2006 : public MLCMST_Improver
{
public:
    static std::string id();

    LocalSearch2006();
    LocalSearch2006(std::unique_ptr< MLCMST_Solver > init_solver, std::unique_ptr< MLCMST_Solver > subnet_solver);
    ~LocalSearch2006() override;

    network::MLCMST improve(network::MLCMST mlcmst, const network::MLCCNetwork &mlcc_network) override;
    std::vector<int> improvementStep(const network::MLCCNetwork& network, std::vector<int> group_ids);

private:
    using MLCMST = network::MLCMST;
    using MLCCNetwork = network::MLCCNetwork;
    using Network = network::Network;

    static double EPS_;

    MLCMST_SubnetSolver subnet_solver_;

    const MLCCNetwork* network_ = nullptr;

    /**
     * Graph structure :
     * 0 - N-1 --> regular nodes
     * N - 2N-1 --> pseudo nodes (valid only group_id + N, rest isolated vertices)
     * 2N --> origin node
     * @return Neighbourhood graph
     */
    Network buildNeighbourhoodGraph(const std::vector<int>& group_id);

    std::unordered_map<int, int> groupDemands(const std::vector<int>& group_id);
    std::unordered_map<int, std::vector<int>> groups(const std::vector<int>& group_id);
    static std::optional<std::pair<std::vector<int>, double>> findBestProfitableExchange(int s, const Network& net,
            const std::vector<int>& group_id);

    std::optional<std::vector<int>> findBestProfitableExchange(const std::vector<int>& group_id);
    static void implementExchange(std::vector<int> exchange, std::vector<int>& group_id);

};

}
