#include <network/capacitated_network.hpp>



namespace MLCMST {
namespace network {

CapacitatedNetwork::CapacitatedNetwork(const Network& network, int edge_capacity) :
    network(network), edge_capacity(edge_capacity)
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
