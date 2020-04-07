#pragma once

#include <vector>

#include <network/network.hpp>

namespace MLCMST {
namespace network {

class CapacitatedNetwork
{
    Network network;
    int edge_capacity;

public:
    CapacitatedNetwork(const Network& network, int edge_capacity);
    ~CapacitatedNetwork();

    int getEdgeCapacity() const;
    const Network& getNetwork() const;
};

}
}
