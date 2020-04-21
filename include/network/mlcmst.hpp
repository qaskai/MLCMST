#pragma once

#include <functional>
#include <vector>

#include <network/mlcc_network.hpp>

namespace MLCMST::network {

class MLCMST final
{
public:
    MLCMST(const MLCCNetwork& network, std::vector<int> parents, std::vector<int> edge_levels);
    ~MLCMST();

    int parent(int v) const;
    int edgeLevel(int v) const;
    double cost() const;

private:
    std::reference_wrapper<const MLCCNetwork> _network;
    std::vector<int> _parents;
    std::vector<int> _edge_levels;
    double _cost;

    double calculateCost();
};

}
