#include <model/multi_level_centralized_capacitated_network.hpp>


MultiLevelCentralizedCapacitatedNetwork::MultiLevelCentralizedCapacitatedNetwork(
    int center, const std::vector<CapacitatedNetwork>& networks) : center(center), networks(networks)
{

}


MultiLevelCentralizedCapacitatedNetwork::~MultiLevelCentralizedCapacitatedNetwork()
{

}