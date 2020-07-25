#include <heuristic/link_upgrade.hpp>

#include <assert.h>

#include <functional>
#include <algorithm>
#include <iostream>
#include <set>
#include <unordered_set>

#include <algorithm>
#include <numeric>
#include <chrono>
#include <tuple>
#include <util/util.hpp>

namespace MLCMST::heuristic {

std::string LinkUpgrade::id()
{
    static const std::string id = "link_upgrade";
    return id;
}

LinkUpgrade::LinkUpgrade(LinkUpgrade::Params params) : params(params) {
}

LinkUpgrade::~LinkUpgrade() = default;

MLCMSTSolver::Result LinkUpgrade::solve(const network::MLCCNetwork &mlcc_network)
{
    network_ = &mlcc_network;

    auto time_start = std::chrono::high_resolution_clock::now();

    network::MLCMST mlcmst = mainLoop();

    auto time_end = std::chrono::high_resolution_clock::now();
    double wall_time = std::chrono::duration<double, std::milli>(time_end - time_start).count();

    return Result(
        mlcmst,
        std::nullopt,
        wall_time,
        true
    );
}

network::MLCMST LinkUpgrade::mainLoop()
{
    struct Upgrade {
        double saving;
        int i, l;
        std::vector<int> H;
    };

    network::MLCMST mlcmst = network::MLCMST::star(*network_);
    std::vector<int> upgrade_candidates = network_->regularVertexSet();

    auto viable_upgrade_set = [&] (int i) -> std::vector<int> {
        std::vector<int> available_levels(network_->levelsNumber() - mlcmst.edgeLevel(i));
        std::iota(available_levels.begin(), available_levels.end(), mlcmst.edgeLevel(i));
        return available_levels;
    };
    auto loop = [&] (int set_level) -> void {
        while (true) {
            Upgrade best_upgrade{ .saving=1e-9, .i=-1 };
            auto update_best_upgrade = [&] (int i, int l) {
                auto[saving, H] = findUpgradeSetH(i, l, mlcmst);
                if (best_upgrade.saving < saving) {
                    best_upgrade = Upgrade{
                        .saving = saving, .i = i, .l = l, .H = H
                    };
                }
            };

            for (int i : upgrade_candidates) {
                if (params.check_all_link_types) {
                    for (int l : viable_upgrade_set(i)) {
                        update_best_upgrade(i, l);
                    }
                } else {
                    if (mlcmst.edgeLevel(i) <= set_level)
                        update_best_upgrade(i, set_level);
                }
            }
            if (best_upgrade.i == -1)
                break;
            implementUpgrade(mlcmst, best_upgrade.i, best_upgrade.l, best_upgrade.H);
            if (!params.reupgrade_nodes) {
                util::erase(upgrade_candidates, best_upgrade.i);
            }
        }
    };

    if (params.check_all_link_types) {
        loop(-1);
    } else {
        for (int l = network_->levelsNumber()-1; l >= 0; l--) {
            loop(l);
        }
    }

    return mlcmst;
}

void LinkUpgrade::implementUpgrade(network::MLCMST& mlcmst, int i, int level, const std::vector<int> &H)
{
    mlcmst.edgeLevel(i) = level;
    for (int j : H) {
        mlcmst.parent(j) = i;
    }
}

std::pair<double, std::vector<int>> LinkUpgrade::findUpgradeSetH(int i, int level, network::MLCMST mlcmst)
{
    double saving_amount =
            network_->edgeCost(i, mlcmst.parent(i), mlcmst.edgeLevel(i))
            - network_->edgeCost(i, mlcmst.parent(i), level);
    mlcmst.edgeLevel(i) = level;

    std::vector<int> loads = mlcmst.loads(*network_);
    std::vector<int> subnet = mlcmst.subnet();

    auto actual_saving = [&] (int j) { return d(i,j , mlcmst); };
    auto modified_saving = [&] (int j) { return modified_d(i, j, mlcmst, loads); };

    auto candidates_cmp = [&] (int a, int b) {
        double v1 = modified_saving(a), v2 = modified_saving(b);
        return v1 != v2 ? v1 > v2 : a < b;
    };
    std::set<int, decltype(candidates_cmp)> candidates(candidates_cmp);
    if (params.H_leafs_only) {
        auto leafs = mlcmst.leafs();
        candidates.insert(leafs.begin(), leafs.end());
    } else {
        auto terminal_nodes = network_->regularVertexSet();
        candidates.insert(terminal_nodes.begin(), terminal_nodes.end());
        for (int x : getPathToRoot(i, mlcmst)) {
            candidates.erase(x);
        }
    }
    candidates.erase(i);

    auto canBeAttached = [&] (int child) -> bool {
        std::vector<int> node_path = getPathToRoot(i, mlcmst);
        std::vector<int> child_path = getPathToRoot(child, mlcmst);
        std::tie(node_path, child_path) = shaveCommonSuffix(node_path, child_path);

        int child_load = loads[child];
        for (int v : node_path) {
            if (loads[v] + child_load > network_->edgeCapacity(mlcmst.edgeLevel(v))) {
                return false;
            }
        }
        return true;
    };

    auto setLoad = [&] (int v, int new_load) {
        if (candidates.count(v)) {
            candidates.erase(v);
            loads[v] = new_load;
            candidates.insert(v);
        } else {
            loads[v] = new_load;
        }
    };
    auto addLoadOnPath = [&] (const std::vector<int>& vec, int load) -> void {
        for (int v : vec) {
            setLoad(v, loads[v] + load);
        }
    };
    std::vector<std::unordered_set<int>> children;
    {
        auto children_lists = mlcmst.childrenLists();
        std::transform(children_lists.begin(), children_lists.end(), std::back_inserter(children),
                [] (const std::vector<int>& vec) { return std::unordered_set<int>(vec.begin(), vec.end()); });
    }
    auto subtreeVertices = [&] (int v) {
        std::vector<int> subtree_vertices;
        std::function<void(int)> dfs = [&] (int i) {
            subtree_vertices.push_back(i);
            std::for_each(children[i].begin(), children[i].end(), dfs);
        };
        dfs(v);
        return subtree_vertices;
    };
    auto attach = [&] (int child) -> void {
        std::vector<int> node_path = getPathToRoot(i, mlcmst);
        std::vector<int> child_path = getPathToRoot(child, mlcmst);
        std::tie(node_path, child_path) = shaveCommonSuffix(node_path, child_path);

        int new_load = loads[child];
        addLoadOnPath(node_path, +new_load);
        child_path.erase(child_path.begin());
        addLoadOnPath(child_path, -new_load);

        children[mlcmst.parent(child)].erase(child);
        children[i].insert(child);
        mlcmst.parent(child) = i;
        subnet[child] = subnet[i];

        auto subtree_vertices = subtreeVertices(child);
        for (int v : subtree_vertices) {
            candidates.erase(v);
        }
    };

    std::vector<int> H;
    while (!candidates.empty()) {
        auto it = candidates.begin();
        int c = *it;
        candidates.erase(it);

        double s = actual_saving(c);
        if (s < 1e-9)
            continue;

        if (canBeAttached(c)) {
            attach(c);
            saving_amount += s;
            H.push_back(c);
        }
    }
    return {saving_amount, H};
}

double LinkUpgrade::d(int i, int j, const network::MLCMST &mlcmst)
{
    int level = mlcmst.edgeLevel(j);
    return
            network_->edgeCost(j, mlcmst.parent(j), level) -
            network_->edgeCost(j, i, level);
}

double LinkUpgrade::modified_d(int i, int j, const network::MLCMST& mlcmst, const std::vector<int> &load)
{
    return d(i,j, mlcmst) / load[j];
}

std::vector<int> LinkUpgrade::getPathToRoot(int node, const network::MLCMST &mlcmst)
{
    std::vector<int> path { node };
    while (path.back() != mlcmst.root()) {
        path.push_back(mlcmst.parent(path.back()));
    }
    path.pop_back();
    return path;
}

std::pair<std::vector<int>, std::vector<int>> LinkUpgrade::shaveCommonSuffix(std::vector<int> v1, std::vector<int> v2)
{
    while (!v1.empty() && !v2.empty() && v1.back() == v2.back()) {
        v1.pop_back();
        v2.pop_back();
    }
    return std::make_pair(v1, v2);
}

}
