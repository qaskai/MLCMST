#include <network/mlcmst.hpp>

#include <utility>
#include <numeric>
#include <functional>

#include <util/util.hpp>

namespace MLCMST::network {

MLCMST::MLCMST(int N, int root) : _root(root), _parents(N, root), _edge_levels(N, 0)
{
}

MLCMST::~MLCMST() = default;

unsigned MLCMST::vertexCount() const
{
    return _parents.size();
}

std::vector<int> MLCMST::vertexSet() const
{
    return util::firstN(vertexCount());
}

int& MLCMST::parent(int v)
{
    return _parents[v];
}

int MLCMST::parent(int v) const
{
    return _parents[v];
}

const std::vector<int> &MLCMST::parents() const
{
    return _parents;
}

int& MLCMST::edgeLevel(int v)
{
    return _edge_levels[v];
}

int MLCMST::edgeLevel(int v) const
{
    return _edge_levels[v];
}

const std::vector<int> &MLCMST::edgeLevels() const
{
    return _edge_levels;
}

double MLCMST::cost(const MLCCNetwork& mlcc_network) const
{
    double cost = 0;
    for (int i=0; i < mlcc_network.vertexCount(); i++) {
        if (i == _root)
            continue;
        cost += mlcc_network.edgeCost(i, parent(i), edgeLevel(i));
    }
    return cost;
}

int MLCMST::root() const
{
    return _root;
}

std::vector<std::vector<int>> MLCMST::childrenLists() const
{
    std::vector<std::vector<int>> children(vertexCount());
    for (int v : vertexSet()) {
        int p = parent(v);
        if (v != p)
            children[p].push_back(v);
    }
    return children;
}

std::vector<int> MLCMST::leafs() const
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

std::vector<int> MLCMST::subtreeVertices(int v) const
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

std::vector<int> MLCMST::loads(const MLCCNetwork &network) const
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

bool MLCMST::checkValidity(const MLCCNetwork &network) const
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

std::vector<int> MLCMST::slack(const MLCCNetwork &network) const
{
    std::vector<int> slack(network.vertexCount());
    std::vector<int> load = loads(network);

    for (int i : network.vertexSet()) {
        slack[i] = network.edgeCapacity(edgeLevel(i)) - load[i];
    }
    slack[network.center()] = network.vertexCount();
    return slack;
}

std::vector<int> MLCMST::reserves(const MLCCNetwork &network) const
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

std::vector<int> MLCMST::subnet() const
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

MLCMST MLCMST::star(const MLCCNetwork& network)
{
    MLCMST mlcmst(network.vertexCount(), network.center());
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
