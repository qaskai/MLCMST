#pragma once

#include <vector>

#include <model/network.hpp>


class MultiLevelNetwork
{
    int center;
    std::vector<Network> networks;

public:
    MultiLevelNetwork(const std::vector<Network>& networks);
    ~MultiLevelNetwork();
    
};