#include <network/network.hpp>

namespace MLCMST {
namespace network {

Network::Network(const std::vector<std::vector<double>>& costs) : costs(costs)
{

}

Network::~Network()
{

}

int Network::getSize() const
{
    return costs.size();
}

double& Network::edgeCost(int v, int w)
{
    return costs[v][w];
}

const double& Network::edgeCost(int v, int w) const
{
    return costs[v][w];
}

}
}
