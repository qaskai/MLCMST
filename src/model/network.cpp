#include <model/network.hpp>

Network::Network(std::vector<std::vector<int>> costs, int center, int edge_capacity) :
    costs(costs), center(center), edge_capacity(edge_capacity)
{
    
}

Network::~Network()
{

}


int Network::getCenter() const
{
    return center;
}

int Network::getEdgeCapacity() const
{
    return edge_capacity;
}