#include <network/mlcc_network.hpp>



namespace MLCMST {
namespace network {

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

}
}
