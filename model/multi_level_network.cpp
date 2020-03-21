#include "multi_level_network.h"


MultiLevelNetwork::MultiLevelNetwork(const std::vector<Network>& networks) : networks(networks)
{
    if (networks.empty()) {
        throw "Cannot crete empty multi level network";
    }
    this->center = networks[0].getCenter();
    for (const Network& n : networks) {
        if (n.getCenter() != this->center) {
            throw "Centers in provided netowrks do not match";
        }
    }
}


MultiLevelNetwork::~MultiLevelNetwork()
{

}