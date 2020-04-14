#include <network/capacitated_network.hpp>



namespace MLCMST {
namespace network {

CapacitatedNetwork::CapacitatedNetwork(int edge_capacity, const Network& network)
    : edge_capacity(edge_capacity), network(network)
{
    
}

CapacitatedNetwork::~CapacitatedNetwork()
{

}


int CapacitatedNetwork::getSize() const
{
    return network.getSize();
}

int CapacitatedNetwork::getEdgeCapacity() const
{
    return edge_capacity;
}

const Network& CapacitatedNetwork::getNetwork() const
{
    return network;
}

}
}
