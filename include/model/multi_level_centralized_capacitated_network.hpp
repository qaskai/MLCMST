#pragma once

#include <vector>

#include <model/capacitated_network.hpp>


class MultiLevelCentralizedCapacitatedNetwork
{
    int center;
    std::vector<CapacitatedNetwork> networks;

public:
    MultiLevelCentralizedCapacitatedNetwork(int center, const std::vector<CapacitatedNetwork>& networks);
    ~MultiLevelCentralizedCapacitatedNetwork();
    
};