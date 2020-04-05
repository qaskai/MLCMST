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


int CapacitatedNetwork::getEdgeCapacity() const
{
    return edge_capacity;
}

}
}
