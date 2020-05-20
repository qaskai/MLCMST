#include <network/mlcmst.hpp>

#include <utility>
#include <numeric>
#include <functional>

#include <util/util.hpp>

namespace MLCMST::network {

MLCMST::MLCMST(unsigned int N, unsigned root) : _root(root), _parents(N, root), _edge_levels(N, 0)
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

std::vector<std::vector<int>> MLCMST::getChildrenLists() const
{
    std::vector<std::vector<int>> children(vertexCount());
    for (int v : vertexSet()) {
        int p = parent(v);
        if (v != p)
            children[p].push_back(v);
    }
    return children;
}

std::vector<int> MLCMST::getLoads(const MLCCNetwork &network) const
{
    std::vector<int> load(vertexCount());
    auto children = getChildrenLists();
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
    std::vector<int> loads = getLoads(network);
    if (_root != network.center()) {
        return false;
    }
    for (int i : vertexSet()) {
        if (i != _root && loads[i] > network.edgeCapacity(edgeLevel(i))) {
            return false;
        }
    }
    return true;
}


}
