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

double& CapacitatedNetwork::edgeCost(int i, int j)
{
    return _network.edgeCost(i,j);
}

double CapacitatedNetwork::edgeCost(int i, int j) const
{
    return _network.edgeCost(i,j);
}

const Network& CapacitatedNetwork::network() const
{
    return _network;
}

std::pair<CapacitatedNetwork, std::vector<int>> CapacitatedNetwork::subNetwork(const std::vector<int> &vertices) const
{
    auto [ subNet, mapping ] = _network.subNetwork(vertices);
    return std::make_pair(CapacitatedNetwork(_edge_capacity, subNet), mapping);
}

double CapacitatedNetwork::infinity()
{
    return Network::infinity();
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

CapacitatedNetwork operator*(const CapacitatedNetwork& n, double scalar)
{
    return CapacitatedNetwork(n.edgeCapacity(), scalar * n.network());
}

CapacitatedNetwork operator*(double scalar, const CapacitatedNetwork& n)
{
    return n*scalar;
}

}
