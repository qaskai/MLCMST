#pragma once

#include <vector>

#include <network/network.hpp>

namespace MLCMST::network {

class CapacitatedNetwork final
{
public:
    CapacitatedNetwork(int edge_capacity, const Network& network);
    ~CapacitatedNetwork();

    [[nodiscard]] int vertexCount() const;
    [[nodiscard]] int edgeCapacity() const;
    double& edgeCost(int i, int j);
    [[nodiscard]] double edgeCost(int i, int j) const;
    [[nodiscard]] const Network& network() const;

    static double infinity();
    [[nodiscard]] std::pair<CapacitatedNetwork, std::vector<int>> subNetwork(const std::vector<int>& vertices) const;

private:
    int _edge_capacity;
    Network _network;
};

bool operator==(const CapacitatedNetwork& cn1, const CapacitatedNetwork& cn2);
bool operator!=(const CapacitatedNetwork& cn1, const CapacitatedNetwork& cn2);
CapacitatedNetwork operator*(const CapacitatedNetwork& n, double scalar);
CapacitatedNetwork operator*(double scalar, const CapacitatedNetwork& n);

}
