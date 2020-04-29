#include <network/mlcc_network.hpp>


namespace MLCMST::network {

MLCCNetwork::MLCCNetwork(
    int center,
    const std::vector<CapacitatedNetwork>& networks,
    const std::vector<int>& demands
)
    : _center(center), _networks(networks), _demands(demands)
{

}

MLCCNetwork::~MLCCNetwork() = default;

int MLCCNetwork::vertexCount() const
{
    return _networks[0].vertexCount();
}

int MLCCNetwork::center() const
{
    return _center;
}
    
int MLCCNetwork::levelsNumber() const
{
    return _networks.size();
}

int MLCCNetwork::demand(int v) const
{
    return _demands[v];
}

const Network& MLCCNetwork::network(int i) const
{
    return _networks[i].network();
}

int MLCCNetwork::edgeCapacity(int network_i) const
{
    return _networks[network_i].edgeCapacity();
}

bool operator==(const MLCCNetwork& n1, const MLCCNetwork n2)
{
    if (n1.vertexCount() != n2.vertexCount())
        return false;
    if (n1.center() != n2.center())
        return false;
    if (n1.levelsNumber() != n2.levelsNumber())
        return false;
    for (int i=0; i < n1.vertexCount(); i++) {
        if (n1.demand(i) != n2.demand(i))
            return false;
    }
    for (int l=0; l < n1.levelsNumber(); l++) {
        if (n1.network(l) != n2.network(l))
            return false;
    }

    return true;
}

bool operator!=(const MLCCNetwork& n1, const MLCCNetwork n2)
{
    return !(n1 == n2);
}

}
