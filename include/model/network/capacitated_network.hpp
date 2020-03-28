#pragma once

#include <vector>


namespace MLCMST {
namespace model {
namespace network {

class CapacitatedNetwork
{
    int edge_capacity;
    std::vector<std::vector<double>> costs;

public:
    CapacitatedNetwork(int edge_capacity, std::vector<std::vector<double>> costs);
    ~CapacitatedNetwork();

    int getEdgeCapacity() const;
};

}
}
}
