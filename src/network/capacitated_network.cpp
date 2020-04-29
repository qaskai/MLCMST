#include <network/capacitated_network.hpp>


namespace MLCMST::network {

CapacitatedNetwork::CapacitatedNetwork(int edge_capacity, const Network& network)
    : _edge_capacity(edge_capacity), _network(network)
{
    
}

CapacitatedNetwork::~CapacitatedNetwork() = default;


int CapacitatedNetwork::vertexCount() const
{
    return _network.vertexCount();
}

int CapacitatedNetwork::edgeCapacity() const
{
    return _edge_capacity;
}

const Network& CapacitatedNetwork::network() const
{
    return _network;
}


bool operator==(const CapacitatedNetwork& cn1, const CapacitatedNetwork& cn2)
{
    if (cn1.edgeCapacity() != cn2.edgeCapacity())
        return false;
    return cn1.network() == cn2.network();
}

bool operator!=(const CapacitatedNetwork& cn1, const CapacitatedNetwork& cn2)
{
    return !(cn1 == cn2);
}

}
