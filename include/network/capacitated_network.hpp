#pragma once

#include <vector>

#include <network/network.hpp>

namespace MLCMST {
namespace network {

class CapacitatedNetwork final
{
    int edge_capacity;
    Network network;

public:
    CapacitatedNetwork(int edge_capacity, const Network& network);
    ~CapacitatedNetwork();

    int getSize() const;
    int getEdgeCapacity() const;
    const Network& getNetwork() const;
};

}
}
