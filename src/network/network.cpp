#include <network/network.hpp>

#include <cmath>

namespace MLCMST::network {

Network::Network(const std::vector<std::vector<double>>& costs) : _costs(costs)
{

}

Network::~Network() = default;

int Network::vertexCount() const
{
    return _costs.size();
}

const std::vector<std::vector<double>>& Network::costs() const
{
    return _costs;
}

double& Network::edgeCost(int v, int w)
{
    return _costs[v][w];
}

const double& Network::edgeCost(int v, int w) const
{
    return _costs[v][w];
}


bool operator==(const Network& n1, const Network& n2)
{
    if (n1.vertexCount() != n2.vertexCount()) {
        return false;
    }
    const double EPS = 1e-9;

    int size = n1.vertexCount();
    for (int i=0; i<size; i++) {
        for (int j=0; j<size; j++) {
            if (std::abs(n1.edgeCost(i,j) - n2.edgeCost(i,j)) > EPS)
                return false;
        }
    }
    return true;
}

bool operator!=(const Network& n1, const Network& n2)
{
    return !(n1 == n2);
}

}
