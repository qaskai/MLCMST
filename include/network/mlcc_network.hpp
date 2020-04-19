#pragma once

#include <vector>

#include <network/capacitated_network.hpp>


namespace MLCMST {
namespace network {

/**
 * MLCC stands for Multi Level Centralized Capacitated
 * 
 */
class MLCCNetwork final
{
public:
    MLCCNetwork(int center, const std::vector<CapacitatedNetwork>& networks, const std::vector<int>& demands);
    ~MLCCNetwork();

    int size() const;
    int center() const;
    int levelsNumber() const;
    int demand(int v) const;
    const Network& network(int i) const;
    int edgeCapacity(int network_i) const;

private:
    int _center;
    std::vector<CapacitatedNetwork> _networks;
    std::vector<int> _demands;

};

}
}
