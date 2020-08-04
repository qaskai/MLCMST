#pragma once

#include <vector>
#include <tuple>

#include <mlcmst_solver.hpp>
#include <heuristic/improvement/mlcmst_improver.hpp>

#include <json/property.hpp>

namespace MLCMST::heuristic::improvement {

class LinkUpgrade : public MLCMST_Improver
{
public:
    static std::string id();

    struct Params {
        bool check_all_link_types; // if true will iterate over viable link upgrades to find best H, if not L,L-1,...,0
        bool reupgrade_nodes;
        bool H_leafs_only;

        constexpr static auto properties = std::make_tuple(
            json::Property<Params,bool>{&Params::check_all_link_types, "check_all_link_types"},
            json::Property<Params,bool>{&Params::reupgrade_nodes, "reupgrade_nodes"},
            json::Property<Params,bool>{&Params::H_leafs_only, "H_leafs_only"}
            );
    };
    explicit LinkUpgrade(Params params);
    LinkUpgrade(std::unique_ptr< MLCMST_Solver > init_solver, Params params);
    ~LinkUpgrade() override;

    network::MLCMST improve(network::MLCMST mlcmst, const network::MLCCNetwork &mlcc_network) override;

private:
    Params params;

    const network::MLCCNetwork* network_ = nullptr;

    network::MLCMST mainLoop(network::MLCMST mlcmst);

    std::pair<double, std::vector<int>> findUpgradeSetH(int i, int level, network::MLCMST mlcmst);
    static void implementUpgrade(network::MLCMST& mlcmst, int i, int level, const std::vector<int>& H);

    double d(int i, int j, const network::MLCMST& mlcmst);
    double modified_d(int i, int j, const network::MLCMST& mlcmst, const std::vector<int>& load);

    static std::vector<int> getPathToRoot(int node, const network::MLCMST &mlcmst);
    static std::pair<std::vector<int>, std::vector<int>> shaveCommonSuffix(std::vector<int> v1, std::vector<int> v2);
};

}
