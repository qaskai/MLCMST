#include <model/network/capacitated_network.hpp>



namespace MLCMST {
namespace model {
namespace network {

CapacitatedNetwork::CapacitatedNetwork(int edge_capacity, std::vector<std::vector<double>> costs) :
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

}
}
}
