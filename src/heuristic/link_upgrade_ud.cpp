#include <heuristic/link_upgrade_ud.hpp>

#include <stack>
#include <queue>
#include <algorithm>
#include <chrono>
#include <stdexcept>
#include <numeric>

#include <network/mlcmst.hpp>

namespace MLCMST::heuristic {

std::string LinkUpgradeUD::id()
{
    static std::string id = "link_upgrade_UD";
    return id;
}

LinkUpgradeUD::~LinkUpgradeUD() = default;

void LinkUpgradeUD::checkDemands(const MLCCNetwork& mlcc_network)
{
    for (int i=0; i < mlcc_network.vertexCount(); i++) {
        if (i != mlcc_network.center() && mlcc_network.demand(i) != 1) {
            throw std::invalid_argument("LinkUpgradeUnitDemand accepts only networks with unit vertex demands");
        }
    }
}

MLCMSTSolver::Result LinkUpgradeUD::solve(const network::MLCCNetwork &mlcc_network)
{
    checkDemands(mlcc_network);

    auto time_start = std::chrono::high_resolution_clock::now();
    network_ = &mlcc_network;
    MLCMST mlcmst = MLCMST::star(mlcc_network.vertexCount(), mlcc_network.center());

    free_vertices_.resize(network_->vertexCount());
    std::iota(free_vertices_.begin(), free_vertices_.end(), 0);
    free_vertices_.erase(std::find(free_vertices_.begin(), free_vertices_.end(), network_->center()));

    for (int l = mlcc_network.levelsNumber()-1; l >= 0; l--) {
        while ( !free_vertices_.empty() && step(l, mlcmst));
    }

    auto time_end = std::chrono::high_resolution_clock::now();
    double wall_time = std::chrono::duration<double, std::milli>(time_end - time_start).count();

    return Result(
            mlcmst,
            std::nullopt,
            wall_time,
            true
    );
}

bool LinkUpgradeUD::step(int level, MLCMST &mlcmst)
{
    double max_saving = 0;
    int max_saving_node = -1;
    for (int i : free_vertices_) {
        double saving;
        std::tie(saving, std::ignore) = computeSavings(level, i, mlcmst);
        if (saving > max_saving) {
            max_saving = saving;
            max_saving_node = i;
        }
    }

    if (max_saving_node == -1) {
        return false;
    } else {
        std::vector<int> new_children;
        std::tie(std::ignore, new_children) = computeSavings(level, max_saving_node, mlcmst);
        mlcmst.edgeLevel(max_saving_node) = level;
        for (int c : new_children) {
            mlcmst.parent(c) = max_saving_node;
        }
        free_vertices_.erase(std::find(free_vertices_.begin(), free_vertices_.end(), max_saving_node));
        return true;
    }
}

std::pair<double, std::vector<int>> LinkUpgradeUD::computeSavings(int level, int node, MLCMST mlcmst)
{
    double saving_amount = 0;
    saving_amount += network_->edgeCost(node, mlcmst.parent(node), mlcmst.edgeLevel(node))
            - network_->edgeCost(node, mlcmst.parent(node), level);

    mlcmst.edgeLevel(node) = level;
    std::vector<int> slack = mlcmst.slack(*network_);
    std::vector<int> res = mlcmst.reserves(*network_);
    std::vector<int> subnet = mlcmst.subnet();

    auto saving = [&] (int potential_child) -> double {
        double old_edge = network_->edgeCost(potential_child, mlcmst.parent(potential_child), 0);
        double potential_edge = network_->edgeCost(potential_child, node, 0);
        return old_edge - potential_edge;
    };

    std::vector<int> candidates = mlcmst.leafs();
    candidates.erase(std::find(candidates.begin(), candidates.end(), node));
    std::sort(candidates.begin(), candidates.end(), [&] (int c1, int c2) {
        return saving(c1) > saving(c2);
    });


    std::vector<int> children;
    for (int c : candidates) {
        double s = saving(c);
        if (s <= 0.)
            break;
        if (canBeAttached(node, c, mlcmst, res, subnet, slack)) {
            attach(node, c, mlcmst, res, subnet, slack);
            children.push_back(c);
            saving_amount += s;
        }
    }
    return std::make_pair(saving_amount, children);
}

std::vector<int> LinkUpgradeUD::getPathToRoot(int node, const MLCMST &mlcmst) {
    std::vector<int> path { node };
    while (path.back() != network_->center()) {
        path.push_back(mlcmst.parent(path.back()));
    }
    return path;
}

std::pair<std::vector<int>, std::vector<int>>
LinkUpgradeUD::shaveCommonSuffix(std::vector<int> v1, std::vector<int> v2)
{
    while (!v1.empty() && !v2.empty() && v1.back() == v2.back()) {
        v1.pop_back();
        v2.pop_back();
    }
    return std::make_pair(v1, v2);
}

bool
LinkUpgradeUD::canBeAttached(int node, int potential_child, const MLCMST &mlcmst, const std::vector<int> &res,
                             const std::vector<int> &subnet, const std::vector<int> &slack)
{
    if (res[node] > 0) {
        return true;
    } else if (subnet[node] == subnet[potential_child]) {
        std::vector<int> node_path = getPathToRoot(node, mlcmst);
        std::vector<int> potential_child_path = getPathToRoot(potential_child, mlcmst);
        std::tie(node_path, potential_child_path) = shaveCommonSuffix(node_path, potential_child_path);
        for (int v : potential_child_path) {
            if (slack[v] <= res[v])
                return false;
        }
        return true;
    } else {
        return false;
    }
}

void
LinkUpgradeUD::attach(int node, int new_child, MLCMST &mlcmst, std::vector<int> &res, std::vector<int> &subnet,
                      std::vector<int> &slack)
{
    auto change_load_on_path = [&] (const std::vector<int>& path, int c) -> void {
        for (auto it = path.rbegin(); it != path.rend(); it++) {
            int v = *it;
            slack[v] -= c;
            res[v] = std::min(slack[v], res[mlcmst.parent(v)]);
        }
    };

    std::vector<int> node_path = getPathToRoot(node, mlcmst);
    std::vector<int> child_path = getPathToRoot(new_child, mlcmst);
    if (subnet[node] == subnet[new_child]) {
        std::tie(node_path, child_path) = shaveCommonSuffix(node_path, child_path);
    }
    change_load_on_path(node_path, +1);
    change_load_on_path(child_path, -1);

    mlcmst.parent(new_child) = node;
    subnet[new_child] = subnet[node];
    slack[new_child] = network_->edgeCapacity(0) -1;
    res[new_child] = std::min(slack[new_child], res[node]);
}

}
