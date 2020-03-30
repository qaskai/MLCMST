#pragma once

#include <vector>

#include <network/capacitated_network.hpp>


namespace MLCMST {
namespace network {

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

}
}
