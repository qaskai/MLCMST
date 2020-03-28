#pragma once

#include <vector>

#include <model/capacitated_network.hpp>


class MLCentralizedCapacitatedNetwork
{
    int center;
    std::vector<CapacitatedNetwork> networks;
    std::vector<int> demands;

public:
    MLCentralizedCapacitatedNetwork(
        int center, const std::vector<CapacitatedNetwork>& networks, const std::vector<int>& demands);
    ~MLCentralizedCapacitatedNetwork();
    
};