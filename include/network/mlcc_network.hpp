#pragma once

#include <vector>

#include <network/capacitated_network.hpp>


namespace MLCMST {
namespace network {


/**
 * MLCC stands for Multi Level Centralized Capacitated
 * 
 */
class MLCCNetwork
{
    int center;
    std::vector<CapacitatedNetwork> networks;
    std::vector<int> demands;

public:
    MLCCNetwork(
        int center, const std::vector<CapacitatedNetwork>& networks, const std::vector<int>& demands);
    ~MLCCNetwork();
    
    int getSize() const;
};

}
}
