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

int MLCCNetwork::size() const
{
    return _networks[0].size();
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

const CapacitatedNetwork& MLCCNetwork::network(int i) const
{
    return _networks[i];
}

}
}
