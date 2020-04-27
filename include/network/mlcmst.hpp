#pragma once

#include <vector>

#include <network/mlcc_network.hpp>

namespace MLCMST::network {

class MLCMST final
{
public:
    explicit MLCMST(unsigned int N);
    MLCMST(std::vector<int> parents, std::vector<int> edge_levels);
    ~MLCMST();

    int& parent(int v);
    int parent(int v) const;
    int& edgeLevel(int v);
    int edgeLevel(int v) const;
    double cost(const MLCCNetwork& mlcc_network) const;

private:
    std::vector<int> _parents;
    std::vector<int> _edge_levels;
};

}
