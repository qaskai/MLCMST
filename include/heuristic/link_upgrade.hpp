#pragma once

#include <vector>

#include <mlcmst_solver.hpp>

namespace MLCMST::heuristic {

class LinkUpgrade : public MLCMSTSolver
{
public:
    static std::string id();

    struct Params {
        bool H_leafs_only;
        bool check_all_link_types; // if true will iterate over viable link upgrades to find best H, if not L,L-1,...,0
        bool reupgrade_nodes;
    };
    explicit LinkUpgrade(Params params);
    ~LinkUpgrade() override;

    Result solve(const network::MLCCNetwork &mlcc_network) override;

private:
    Params params;

    const network::MLCCNetwork* network_;

    network::MLCMST mainLoop();

    std::pair<double, std::vector<int>> findUpgradeSetH(int i, int level, network::MLCMST mlcmst);
    void implementUpgrade(network::MLCMST& mlcmst, int i, int level, const std::vector<int>& H);

    double d(int i, int j, const network::MLCMST& mlcmst);
    double modified_d(int i, int j, const network::MLCMST& mlcmst, const std::vector<int>& load);

    static std::vector<int> getPathToRoot(int node, const network::MLCMST &mlcmst);
    static std::pair<std::vector<int>, std::vector<int>> shaveCommonSuffix(std::vector<int> v1, std::vector<int> v2);
};

}
