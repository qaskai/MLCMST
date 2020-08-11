#include <network/mlcst.hpp>

#include <cassert>
#include <utility>
#include <numeric>
#include <functional>

#include <util/util.hpp>

namespace MLCMST::network {

MLCST::MLCST(int N, int root) : _root(root), _parents(N, root), _edge_levels(N, 0)
{
}

MLCST::~MLCST() = default;

bool MLCST::setMinimalViableLevels(const MLCCNetwork &mlcc_network)
{
    std::vector<int> levels = mlcc_network.levels();
    std::vector<int> load = loads(mlcc_network);
    auto children = childrenLists();
    std::function<bool(int)> dfs = [&] (int v) {
        auto it = std::lower_bound(levels.begin(), levels.end(), load[v]);
        if (it == levels.end()) {
            return false;
        }
        _edge_levels[v] = std::distance(levels.begin(), it);
        for (int c : children[v]) {
            dfs(c);
        }
        return true;
    };

    bool valid_tree = true;
    for (int c : children[_root]) {
        valid_tree = valid_tree && dfs(c);
    }
    _edge_levels[_root] = 0;
    return valid_tree;
}

unsigned MLCST::vertexCount() const
{
    return _parents.size();
}

std::vector<int> MLCST::vertexSet() const
{
    return util::firstN(vertexCount());
}

int& MLCST::parent(int v)
{
    return _parents[v];
}

int MLCST::parent(int v) const
{
    return _parents[v];
}

const std::vector<int> &MLCST::parents() const
{
    return _parents;
}

int& MLCST::edgeLevel(int v)
{
    return _edge_levels[v];
}

int MLCST::edgeLevel(int v) const
{
    return _edge_levels[v];
}

const std::vector<int> &MLCST::edgeLevels() const
{
    return _edge_levels;
}

double MLCST::cost(const MLCCNetwork& mlcc_network) const
{
    double cost = 0;
    for (int i=0; i < mlcc_network.vertexCount(); i++) {
        if (i == _root)
            continue;
        cost += mlcc_network.edgeCost(i, parent(i), edgeLevel(i));
    }
    return cost;
}

int MLCST::root() const
{
    return _root;
}

std::vector<std::vector<int>> MLCST::childrenLists() const
{
    std::vector<std::vector<int>> children(vertexCount());
    for (int v : vertexSet()) {
        int p = parent(v);
        if (v != p)
            children[p].push_back(v);
    }
    return children;
}

std::vector<int> MLCST::leafs() const
{
    std::vector<std::vector<int>> children = childrenLists();
    std::vector<int> leafs;
    for (int i : vertexSet()) {
        if (children[i].empty()) {
            leafs.push_back(i);
        }
    }
    return leafs;
}

std::vector<int> MLCST::nonLeafTerminals() const
{
    std::vector<int> leafs = this->leafs();
    std::vector<bool> take(vertexCount(), true);
    take[root()] = false;
    for (int l : leafs) {
        take[l] = false;
    }

    std::vector<int> non_leaf_terminals;
    for (int i=0; i<vertexCount(); i++) {
        if (take[i])
            non_leaf_terminals.push_back(i);
    }
    return non_leaf_terminals;
}

std::vector<int> MLCST::subtreeVertices(int v) const
{
    auto children_lists = childrenLists();
    std::vector<int> subtree_vertices;
    std::function<void(int)> dfs = [&] (int i) {
        subtree_vertices.push_back(i);
        std::for_each(children_lists[i].begin(), children_lists[i].end(), dfs);
    };
    dfs(v);
    return subtree_vertices;
}

std::vector<int> MLCST::pathToRoot(int v) const
{
    std::vector<int> path{v};
    while (path.back() != root()) {
        path.push_back(_parents[path.back()]);
    }
    std::reverse(path.begin(), path.end());
    return path;
}

std::vector<int> MLCST::loads(const MLCCNetwork &network) const
{
    std::vector<int> load(vertexCount());
    auto children = childrenLists();
    std::function<int(int)> dfs = [&] (int v) -> int {
        int acc = network.demand(v);
        for (int c : children[v]) {
            acc += dfs(c);
        }
        load[v] = acc;
        return acc;
    };

    load[_root] = 0;
    for (int r : children[_root]) {
        load[_root] += dfs(r);
    }
    return load;
}

bool MLCST::checkValidity(const MLCCNetwork &network) const
{
    std::vector<int> load = loads(network);
    if (_root != network.center()) {
        return false;
    }
    for (int i : vertexSet()) {
        if (i != _root && load[i] > network.edgeCapacity(edgeLevel(i))) {
            return false;
        }
    }
    return true;
}

std::vector<int> MLCST::slack(const MLCCNetwork &network) const
{
    std::vector<int> slack(network.vertexCount());
    std::vector<int> load = loads(network);

    for (int i : network.vertexSet()) {
        slack[i] = network.edgeCapacity(edgeLevel(i)) - load[i];
    }
    slack[network.center()] = network.vertexCount();
    return slack;
}

std::vector<int> MLCST::reserves(const MLCCNetwork &network) const
{
    std::vector<int> res(network.vertexCount());
    auto slack = this->slack(network);
    auto children = childrenLists();
    std::function<void(int)> dfs = [&] (int v) {
        res[v] = std::min(res[parent(v)], slack[v]);
        for (int c : children[v]) {
            dfs(c);
        }
    };

    res[_root] = network.vertexCount();
    for (int r : children[_root]) {
        dfs(r);
    }
    return res;
}

std::vector<int> MLCST::subnet() const
{
    std::vector<int> subnet(vertexCount());
    auto children = childrenLists();
    std::function<void(int)> dfs = [&] (int v) {
        for (int c : children[v]) {
            subnet[c] = subnet[v];
            dfs(c);
        }
    };

    for (int r : children[_root]) {
        subnet[r] = r;
        dfs(r);
    }
    subnet[_root] = _root;
    return subnet;
}

MLCST MLCST::star(const MLCCNetwork& network)
{
    MLCST mlcmst(network.vertexCount(), network.center());
    for (int i=0; i<network.vertexCount(); i++) {
        mlcmst.parent(i) = network.center();
        mlcmst.edgeLevel(i) = 0;
        for (int l = 0; l < network.levelsNumber(); l++) {
            if (network.demand(i) <= network.edgeCapacity(l)) {
                mlcmst.edgeLevel(i) = l;
                break;
            }
        }
    }
    return mlcmst;
}

}
