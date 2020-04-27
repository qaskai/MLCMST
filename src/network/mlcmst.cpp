#include <network/mlcmst.hpp>
#include <utility>

namespace MLCMST::network {

MLCMST::MLCMST(unsigned int N) : _parents(N), _edge_levels(N)
{
}

MLCMST::MLCMST(std::vector<int> parents, std::vector<int> edge_levels)
    : _parents(std::move(parents)), _edge_levels(std::move(edge_levels))
{
}

MLCMST::~MLCMST() = default;

int& MLCMST::parent(int v)
{
    return _parents[v];
}

int MLCMST::parent(int v) const
{
    return _parents[v];
}

int& MLCMST::edgeLevel(int v)
{
    return _edge_levels[v];
}

int MLCMST::edgeLevel(int v) const
{
    return _edge_levels[v];
}

double MLCMST::cost(const MLCCNetwork& mlcc_network) const
{
    double cost = 0;
    for (int i=0; i < mlcc_network.vertexCount(); i++) {
        if (i == mlcc_network.center())
            continue;

        cost += mlcc_network.network(_edge_levels[i]).edgeCost(i, _parents[i]);
    }
    return cost;
}


}
