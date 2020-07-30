#include <heuristic/martins2008_local_search.hpp>

#include <cassert>

#include <algorithm>
#include <limits>

#include <util/util.hpp>

namespace MLCMST::heuristic {

std::string Martins2008_LocalSearch::id()
{
    static const std::string id = "martins2008_local_search";
    return id;
}

Martins2008_LocalSearch::Martins2008_LocalSearch(std::unique_ptr<MLCMST_Solver> subnet_solver,
                                                 Martins2008_LocalSearch::Params params)
        : Martins2008_LocalSearch(std::move(subnet_solver), util::clockMilliseconds(), params)
{
}

Martins2008_LocalSearch::Martins2008_LocalSearch(
        std::unique_ptr<MLCMST_Solver> subnet_solver, long seed, Martins2008_LocalSearch::Params params)
    : params(params), subnet_solver_(std::move(subnet_solver)), int_generator_(0, std::numeric_limits<int>::max(), seed)
{
}

Martins2008_LocalSearch::~Martins2008_LocalSearch() = default;

network::MLCMST Martins2008_LocalSearch::improve(network::MLCMST mlcmst, const network::MLCCNetwork &mlcc_network)
{
    mlcc_network_ = &mlcc_network;

    std::set<int> S = vectorToSet(mlcc_network.regularVertexSet());
    for (int l : mlcmst.leafs()) {
        S.erase(l);
    }
    while (!S.empty()) {
        step(S, mlcmst);
    }
    return mlcmst;
}

void Martins2008_LocalSearch::step(std::set<int> &S, network::MLCMST &mlcmst)
{
    int i = *std::next(S.begin(), int_generator_.generate() % S.size());
    std::vector<int> i_group;
    {
        auto subnet = mlcmst.subnet();
        i_group = util::groupIndexesByValue(subnet)[subnet[i]];
    }
    auto [P, old_cost] = groupSubtrees(i, mlcmst);
    if (P.size() > i_group.size() + 1) {
        MLCMST_SubnetSolver::Result result = subnet_solver_.solveSubnet(*mlcc_network_, P);
        assert(result.mlcmst.checkValidity(mlcc_network_->subNetwork(P).first));
        if (result.cost < old_cost - 1e-9) {
            std::set<int> subtree_leafs = vectorToSet(result.mlcmst.leafs());
            for (int v=0; v<result.mapping.size(); v++) {
                int v_mapped = result.mapping[v];
                if (v_mapped == mlcc_network_->center())
                    continue;

                mlcmst.parent(v_mapped) = result.mapping[result.mlcmst.parent(v)];
                mlcmst.edgeLevel(v_mapped) = result.mlcmst.edgeLevel(v);

                if (subtree_leafs.count(v)) {
                    S.erase(v_mapped);
                } else {
                    S.insert(v_mapped);
                }
            }

        } else {
            for (int v : i_group) {
                S.erase(v);
            }
        }
    } else {
        S.erase(i);
    }
}

std::pair<std::vector<int>, double> Martins2008_LocalSearch::groupSubtrees(int i, const network::MLCMST& mlcmst)
{
    int h = params.h_low + (int_generator_.generate() % (params.h_high - params.h_low + 1));
    auto subnet = mlcmst.subnet();
    auto groups = util::groupIndexesByValue(subnet);

    double old_cost = subtreeCost(groups[subnet[i]], mlcmst);
    std::vector<int> P = groups[subnet[i]];
    P.push_back(mlcc_network_->center());

    std::vector<int> remaining_groups = mlcmst.childrenLists()[mlcmst.root()];
    std::transform(remaining_groups.begin(), remaining_groups.end(), remaining_groups.begin(),
            [&subnet](int v) { return subnet[v]; });
    util::erase(remaining_groups, subnet[i]);
    while (P.size() < h && !remaining_groups.empty()) {
        int g_id = *std::next(remaining_groups.begin(), int_generator_.generate() % remaining_groups.size());
        util::erase(remaining_groups, g_id);
        if (P.size() + groups[g_id].size() <= h)
            continue;

        bool found_improving_edge = false;
        for (int v : groups[g_id]) {
            int level = mlcmst.edgeLevel(v);
            if (mlcc_network_->edgeCost(v, i, level) < mlcc_network_->edgeCost(v, mlcmst.parent(v), level) - 1e-9) {
                found_improving_edge = true;
                break;
            }
        }
        if (found_improving_edge) {
            P.insert(P.end(), groups[g_id].begin(), groups[g_id].end());
            old_cost += subtreeCost(groups[g_id], mlcmst);
        }
    }
    return {P, old_cost};
}

std::set<int> Martins2008_LocalSearch::vectorToSet(const std::vector<int> &v)
{
    return std::set<int>(v.begin(), v.end());
}

double Martins2008_LocalSearch::subtreeCost(const std::vector<int>& vertices, const network::MLCMST &mlcmst)
{
    double cost = 0;
    for (int v : vertices) {
        if (v == mlcmst.root())
            continue;
        cost += mlcc_network_->edgeCost(v, mlcmst.parent(v), mlcmst.edgeLevel(v));
    }
    return cost;
}


}
