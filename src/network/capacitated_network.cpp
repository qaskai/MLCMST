#include <network/capacitated_network.hpp>



namespace MLCMST {
namespace network {

CapacitatedNetwork::CapacitatedNetwork(const std::vector<std::vector<double>>& costs, int edge_capacity) :
    Network(costs), edge_capacity(edge_capacity)
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
