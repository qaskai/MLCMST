#include <network/mlcmst.hpp>
#include <utility>

namespace MLCMST::network {

MLCMST::~MLCMST() = default;

MLCMST::MLCMST(const MLCCNetwork &network, std::vector<int> parents, std::vector<int> edge_levels)
    : _network(network), _parents(std::move(parents)), _edge_levels(std::move(edge_levels))
{
    _cost = calculateCost();
}

double MLCMST::calculateCost() {
    double cost = 0;
    for (int i=0; i < _network.get().vertexCount(); i++) {
        if (i == _network.get().center())
            continue;

        cost += _network.get().network(_edge_levels[i]).edgeCost(i, _parents[i]);
    }
    return cost;
}

int MLCMST::parent(int v) const
{
    return _parents[v];
}

int MLCMST::edgeLevel(int v) const
{
    return _edge_levels[v];
}

double MLCMST::cost() const
{
    return _cost;
}


}
