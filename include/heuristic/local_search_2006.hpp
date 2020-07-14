#pragma once

#include <memory>
#include <optional>
#include <tuple>
#include <unordered_map>

#include <mlcmst_solver.hpp>
#include <mlcmst_subnet_solver.hpp>

#include <network/network.hpp>
#include <network/mlcc_network.hpp>
#include <network/mlcmst.hpp>

namespace MLCMST::heuristic {

class LocalSearch2006 : public MLCMSTSolver
{
public:
    LocalSearch2006();
    LocalSearch2006(std::unique_ptr< MLCMSTSolver > init_solver, std::unique_ptr< MLCMSTSolver > inner_mlcmst_solver);
    ~LocalSearch2006() override;

    Result solve(const network::MLCCNetwork& network) override;

private:
    using MLCMST = network::MLCMST;
    using MLCCNetwork = network::MLCCNetwork;
    using Network = network::Network;

    static double EPS_;

    std::unique_ptr< MLCMSTSolver > init_solver_;
    MLCMST_SubnetSolver subnet_solver_;

    const MLCCNetwork* network_;

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
    std::optional<std::pair<std::vector<int>, double>> findBestProfitableExchange(int s, const Network& net,
            const std::vector<int>& group_id);

    bool step(std::vector<int>& group_id);
    std::optional<std::vector<int>> findBestProfitableExchange(const std::vector<int>& group_id);
    void implementExchange(std::vector<int> exchange, std::vector<int>& group_id);

};

}
