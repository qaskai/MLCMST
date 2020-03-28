#include <model/capacitated_network.hpp>

CapacitatedNetwork::CapacitatedNetwork(int edge_capacity, std::vector<std::vector<int>> costs) :
    edge_capacity(edge_capacity), costs(costs)
{
    
}

CapacitatedNetwork::~CapacitatedNetwork()
{

}


int CapacitatedNetwork::getEdgeCapacity() const
{
    return edge_capacity;
}