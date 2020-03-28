#include <model/ml_centralized_capacitated_network.hpp>


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