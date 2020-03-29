#pragma once

#include <vector>

#include <model/network/network.hpp>

namespace MLCMST {
namespace model {
namespace network {

class CapacitatedNetwork : public Network
{
    int edge_capacity;

public:
    CapacitatedNetwork(const std::vector<std::vector<double>>& costs, int edge_capacity);
    ~CapacitatedNetwork();

    int getEdgeCapacity() const;
};

}
}
}
