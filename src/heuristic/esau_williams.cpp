#include <heuristic/esau_williams.hpp>

#include <algorithm>
#include <set>
#include <unordered_set>
#include <functional>

#include <util/util.hpp>

namespace MLCMST::heuristic {

std::string EsauWilliams::id()
{
    static const std::string id = "esau_williams";
    return id;
}

EsauWilliams::~EsauWilliams() = default;

network::MLCMST EsauWilliams::run(const network::MLCCNetwork &mlcc_network)
{
    const int center = mlcc_network.center();
    std::vector<int> parents = run(
            center, mlcc_network.networks()[mlcc_network.levelsNumber()-1], mlcc_network.demands());
    network::MLCMST mlcmst(mlcc_network.vertexCount(), center);
    for (int i : mlcc_network.vertexSet()) {
        mlcmst.parent(i) = parents[i];
        mlcmst.edgeLevel(i) = mlcc_network.levelsNumber()-1;
    }
    mlcmst.parent(center) = center;
    mlcmst.edgeLevel(center) = 0;
    mlcmst.setMinimalViableLevels(mlcc_network);
    return mlcmst;
}

std::vector<int> EsauWilliams::run(int root, const network::CapacitatedNetwork &network, const std::vector<int>& demands)
{
    const int N = network.vertexCount();
    const int capacity = network.edgeCapacity();

    std::vector<int> loads = demands;
    std::vector<std::vector<int>> sorted_edges(N); // sorted decreasingly
    for (int i=0; i<N; i++) {
        if (i == root)
            continue;
        std::vector<int>& v = sorted_edges[i];
        v = util::firstN(N);
        util::erase(v, i);
        util::erase(v, root);
        std::sort(v.begin(), v.end(), [&network, i] (int a, int b) -> bool {
            return network.edgeCost(i,a) > network.edgeCost(i,b);
        });
    }

    std::vector<int> group_id = util::firstN(N);
    std::vector<std::vector< std::pair<int,int> >> group_edges(N);
    auto createEdge = [](int i, int j) -> std::pair<int,int> {
        if (i > j)
            std::swap(i,j);
        return { i, j };
    };

    auto saving = [&] (int i) {
        return network.edgeCost(i, sorted_edges[i].back()) - network.edgeCost(group_id[i], root);
    };
    auto candidate_cmp = [&] (int i, int j) {
        double c_i = saving(i);
        double c_j = saving(j);
        if ( std::abs(c_i - c_j) < 1e-9 ) {
            return i < j;
        } else {
            return c_i < c_j;
        }
    };
    std::set<int, decltype(candidate_cmp)> candidates(candidate_cmp);
    for (int i=0; i<N; i++) {
        if (i != root)
            candidates.insert(i);
    }

    while (!candidates.empty()) {
        int i = *candidates.begin();
        if (saving(i) > 0) {
            break;
        }
        candidates.erase(i);
        int j = sorted_edges[i].back();
        sorted_edges[i].pop_back();
        if (!sorted_edges[i].empty()) {
            candidates.insert(i);
        }

        const int old_group_id = group_id[i];
        const int new_group_id = group_id[j];
        if (old_group_id != new_group_id && loads[old_group_id] + loads[new_group_id] <= capacity) {
            // merge group i to group j using edge i->j

            std::unordered_set<int> members {i};
            for (auto [a,b] : group_edges[old_group_id]) {
                members.insert(a);
                members.insert(b);
            }
            for (int x : members) {
                candidates.erase(x);
                group_id[x] = new_group_id;
                if (!sorted_edges[x].empty()) {
                    candidates.insert(x);
                }
            }
            group_edges[new_group_id].insert(
                    group_edges[new_group_id].end(), group_edges[old_group_id].begin(), group_edges[old_group_id].end());
            group_edges[new_group_id].push_back(createEdge(i,j));
            group_edges[old_group_id].clear();
            loads[new_group_id] += loads[old_group_id];
        }
    }
    for (int i=0; i<N; i++) {
        if (group_id[i] != i || i == root)
            continue;
        group_edges[i].push_back(createEdge(i,root));
    }

    return directedTreeFromEdges(N, root, util::concat(group_edges));
}

std::vector<int> EsauWilliams::directedTreeFromEdges(int N, int root, const std::vector<std::pair<int, int>> &edges)
{
    std::vector<std::vector<int>> incident_nodes(N);
    for (auto [i,j] : edges) {
        incident_nodes[i].push_back(j);
        incident_nodes[j].push_back(i);
    }

    std::vector<int> parents(N, root);
    std::function<void(int,int)> dfs = [&](int v, int parent) {
        parents[v] = parent;
        for (int x : incident_nodes[v]) {
            if (x == parent)
                continue;
            dfs(x, v);
        }
    };
    dfs(root, root);
    return parents;
}

}
