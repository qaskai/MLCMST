#include "network.hpp"

Network::Network(std::vector<std::vector<int>> costs, int center, int edge_capacity) :
    costs(costs), center(center), edge_capacity(edge_capacity)
{
    
}

Network::~Network()
{

}