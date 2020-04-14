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

const std::vector<std::vector<double>>& Network::getCosts() const
{
    return costs;
}

double& Network::edgeCost(int v, int w)
{
    return costs[v][w];
}

const double& Network::edgeCost(int v, int w) const
{
    return costs[v][w];
}


bool operator==(const Network& n1, const Network& n2)
{
    if (n1.getSize() != n2.getSize()) {
        return false;
    }

    int size = n1.getSize();
    for (int i=0; i<size; i++) {
        for (int j=0; j<size; j++) {
            if (n1.edgeCost(i,j) != n2.edgeCost(i,j))
                return false;
        }
    }
    return true;
}

}
}
