#include <network/network.hpp>

#include <algorithm>
#include <cmath>
#include <utility>
#include <unordered_map>

namespace MLCMST::network {

Network::Network(int N) : _costs(N, std::vector<double>(N, infinity()))
{
}

Network::Network(std::vector<std::vector<double>>  costs) : _costs(std::move(costs))
{

}

Network::Network(std::initializer_list<std::vector<double>> costs) : _costs(costs)
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

bool Network::edgeExists(int v, int w) const
{
    double cost = edgeCost(v,w);
    return cost != infinity() && cost > 1e-9;
}

std::vector<int> Network::neighbourhood(int v) const
{
    std::vector<int> neighbours;
    for (int i = 0; i < vertexCount(); i++) {
        if (i != v && edgeExists(v, i))
            neighbours.push_back(i);
    }
    return neighbours;
}

std::vector<std::vector<int>> Network::neighbourhoods() const
{
    std::vector<std::vector<int>> neighbourhoodLists(vertexCount());
    for (int i=0; i<vertexCount(); i++) {
        neighbourhoodLists[i] = neighbourhood(i);
    }
    return neighbourhoodLists;
}

std::pair<Network, std::vector<int>> Network::subNetwork(const std::vector<int> &vertices) const
{
    std::unordered_map<int,int> rev_mapping;
    std::vector<int> mapping;
    for (int v : vertices) {
        int id = rev_mapping.size();
        rev_mapping[v] = id;
        mapping.push_back(v);
    }
    Network subNet(vertices.size());
    for (int v : vertices) {
        for (int w : vertices) {
            subNet.edgeCost(rev_mapping[v], rev_mapping[w]) = edgeCost(v, w);
        }
    }
    return std::make_pair(subNet, vertices);
}

double Network::infinity()
{
    return INFINITY_;
}

double Network::INFINITY_ = std::numeric_limits<double>::infinity();

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

Network operator*(const Network& n, double scalar)
{
    std::vector<std::vector<double>> costs;
    costs.reserve(n.vertexCount());
    std::transform(n.costs().begin(), n.costs().end(), std::back_inserter(costs),
        [scalar] (std::vector<double> v) {
            for (double& x : v) {
                x*= scalar;
            }
            return v;
        });
    return Network(costs);
}

Network operator*(double scalar, const Network& n)
{
    return n*scalar;
}


}
