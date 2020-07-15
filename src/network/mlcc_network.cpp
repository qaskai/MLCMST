#include <network/mlcc_network.hpp>

#include <algorithm>

#include <util/util.hpp>
#include <utility>

namespace MLCMST::network {

MLCCNetwork::MLCCNetwork(int center, std::vector<CapacitatedNetwork> networks, std::vector<int> demands)
    : _center(center), _networks(std::move(networks)), _demands(std::move(demands))
{
}

MLCCNetwork::~MLCCNetwork() = default;

int MLCCNetwork::vertexCount() const
{
    return _networks[0].vertexCount();
}

int MLCCNetwork::center() const
{
    return _center;
}

int MLCCNetwork::levelsNumber() const
{
    return _networks.size();
}

int MLCCNetwork::demand(int v) const
{
    return _demands[v];
}

const std::vector<int>& MLCCNetwork::demands() const
{
    return _demands;
}

const Network& MLCCNetwork::network(int i) const
{
    return _networks[i].network();
}

const std::vector<CapacitatedNetwork>& MLCCNetwork::networks() const
{
    return _networks;
}

int MLCCNetwork::edgeCapacity(int network_i) const
{
    return _networks[network_i].edgeCapacity();
}

std::vector<int> MLCCNetwork::vertexSet() const
{
    return util::firstN(vertexCount());
}

std::vector<int> MLCCNetwork::regularVertexSet() const
{
    std::vector<int> vs = vertexSet();
    vs.erase(std::find(vs.begin(), vs.end(), center()));
    return vs;
}

double MLCCNetwork::edgeCost(int v, int w, int level) const
{
    return network(level).edgeCost(v, w);
}

std::pair<MLCCNetwork, std::vector<int>> MLCCNetwork::subNetwork(std::vector<int> vertices) const
{
    if (std::find(vertices.begin(), vertices.end(), _center) == vertices.end()) {
        vertices.push_back(_center);
    }
    std::vector<CapacitatedNetwork> sub_networks;
    std::vector<int> mapping = _networks[0].subNetwork(vertices).second;
    for (const CapacitatedNetwork& n : _networks) {
        sub_networks.push_back(n.subNetwork(vertices).first);
    }
    const int sub_center = std::distance(mapping.begin(), std::find(mapping.begin(), mapping.end(), _center));
    std::vector<int> sub_demands;
    sub_demands.reserve(mapping.size());
    for (int x : mapping) {
        sub_demands.push_back(_demands[x]);
    }
    return std::make_pair(MLCCNetwork(sub_center, sub_networks, sub_demands), mapping);
}

MLCCNetwork MLCCNetwork::multiplyEdgeCosts(double scalar) const
{
    std::vector<CapacitatedNetwork> networks = _networks;
    std::transform(networks.begin(), networks.end(), networks.begin(),
        [scalar] (const CapacitatedNetwork& cap_net) {
            return cap_net * scalar;
        });
    return MLCCNetwork(_center, networks, _demands);
}

double MLCCNetwork::infinity()
{
    return Network::infinity();
}

bool operator==(const MLCCNetwork& n1, const MLCCNetwork& n2)
{
    if (n1.vertexCount() != n2.vertexCount())
        return false;
    if (n1.center() != n2.center())
        return false;
    if (n1.levelsNumber() != n2.levelsNumber())
        return false;
    for (int i=0; i < n1.vertexCount(); i++) {
        if (n1.demand(i) != n2.demand(i))
            return false;
    }
    for (int l=0; l < n1.levelsNumber(); l++) {
        if (n1.network(l) != n2.network(l))
            return false;
    }

    return true;
}

bool operator!=(const MLCCNetwork& n1, const MLCCNetwork& n2)
{
    return !(n1 == n2);
}

}
