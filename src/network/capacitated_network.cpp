#include <network/capacitated_network.hpp>



namespace MLCMST {
namespace network {

CapacitatedNetwork::CapacitatedNetwork(int edge_capacity, const Network& network)
    : _edge_capacity(edge_capacity), _network(network)
{
    
}

CapacitatedNetwork::~CapacitatedNetwork() = default;


int CapacitatedNetwork::size() const
{
    return _network.size();
}

int CapacitatedNetwork::edgeCapacity() const
{
    return _edge_capacity;
}

const Network& CapacitatedNetwork::network() const
{
    return _network;
}

}
}
