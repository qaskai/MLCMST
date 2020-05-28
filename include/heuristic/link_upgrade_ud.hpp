#pragma once

#include <vector>
#include <unordered_set>

#include <mlcmst_solver.hpp>

namespace MLCMST::heuristic {

/**
 * Unit demand link upgrade heuristic.
 */
class LinkUpgradeUD : public MLCMSTSolver
{
public:
    ~LinkUpgradeUD() override;

    Result solve(const network::MLCCNetwork &mlcc_network) override;

private:
    using MLCCNetwork = network::MLCCNetwork;
    using MLCMST = network::MLCMST;

    const MLCCNetwork* network_;
    std::vector<int> free_vertices_;

    void checkDemands(const MLCCNetwork& mlcc_network);

    bool step(int level, MLCMST &mlcmst);
    std::pair<double, std::vector<int>> computeSavings(int level, int node, MLCMST mlcmst);

    std::vector<int> getPathToRoot(int node, const MLCMST &mlcmst);
    bool canBeAttached(int node, int potential_child, const MLCMST &mlcmst, const std::vector<int> &res,
                       const std::vector<int> &subnet, const std::vector<int> &slack);
    void attach(int node, int new_child, MLCMST &mlcmst, std::vector<int> &res, std::vector<int> &subnet,
                std::vector<int> &slack);
    std::pair<std::vector<int>, std::vector<int>> shaveCommonSuffix(std::vector<int> v1, std::vector<int> v2);
};

}
