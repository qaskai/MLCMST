#include <model/ml_centralized_capacitated_network.hpp>


MLCentralizedCapacitatedNetwork::MLCentralizedCapacitatedNetwork(
    int center, const std::vector<CapacitatedNetwork>& networks) : center(center), networks(networks)
{

}


MLCentralizedCapacitatedNetwork::~MLCentralizedCapacitatedNetwork()
{

}