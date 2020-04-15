#pragma once

#include <vector>

#include <network/network.hpp>

namespace MLCMST {
namespace network {

class CapacitatedNetwork final
{
public:
    CapacitatedNetwork(int edge_capacity, const Network& network);
    ~CapacitatedNetwork();

    int size() const;
    int edgeCapacity() const;
    const Network& network() const;

private:
    int _edge_capacity;
    Network _network;
};

}
}
