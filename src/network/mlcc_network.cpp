#include <network/mlcc_network.hpp>



namespace MLCMST {
namespace network {

MLCCNetwork::MLCCNetwork(
    int center,
    const std::vector<CapacitatedNetwork>& networks,
    const std::vector<int>& demands
)
    : center(center), networks(networks), demands(demands)
{

}


MLCCNetwork::~MLCCNetwork()
{

}

int MLCCNetwork::getSize() const
{
    return networks[0].getSize();
}

int MLCCNetwork::getCenter() const
{
    return center;
}
    
int MLCCNetwork::getLevelsNumber() const
{
    return networks.size();
}

int MLCCNetwork::getDemand(int v) const
{
    return demands[v];
}

const CapacitatedNetwork& MLCCNetwork::getNetwork(int i) const
{
    return networks[i];
}

}
}
