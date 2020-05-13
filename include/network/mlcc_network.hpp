#pragma once

#include <vector>

#include <network/capacitated_network.hpp>


namespace MLCMST::network {

/**
 * MLCC stands for Multi Level Centralized Capacitated
 * 
 */
class MLCCNetwork final
{
public:
    MLCCNetwork(int center, const std::vector<CapacitatedNetwork>& networks, const std::vector<int>& demands);
    ~MLCCNetwork();

    int center() const;
    int vertexCount() const;
    std::vector<int> vertexSet() const;
    int levelsNumber() const;
    int demand(int v) const;
    const Network& network(int i) const;
    int edgeCapacity(int network_i) const;
    double edgeCost(int v, int w, int level) const;

private:
    int _center;
    std::vector<CapacitatedNetwork> _networks;
    std::vector<int> _demands;

};

bool operator==(const MLCCNetwork& n1, const MLCCNetwork& n2);
bool operator!=(const MLCCNetwork& n1, const MLCCNetwork& n2);

}
