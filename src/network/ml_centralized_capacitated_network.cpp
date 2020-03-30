#include <network/ml_centralized_capacitated_network.hpp>



namespace MLCMST {
namespace network {

MLCentralizedCapacitatedNetwork::MLCentralizedCapacitatedNetwork(
    int center,
    const std::vector<CapacitatedNetwork>& networks,
    const std::vector<int>& demands
) : center(center), networks(networks), demands(demands)
{

}


MLCentralizedCapacitatedNetwork::~MLCentralizedCapacitatedNetwork()
{

}

}
}
