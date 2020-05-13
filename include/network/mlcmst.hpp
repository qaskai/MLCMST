#pragma once

#include <vector>

#include <network/mlcc_network.hpp>

namespace MLCMST::network {

/**
 * MLCMST data class.
 *
 * Assumes that parents vector forms a directed tree and its root (and only the root) has a parent set to itself.
 */
class MLCMST final
{
public:
    MLCMST(unsigned int N, unsigned root);
    ~MLCMST();

    unsigned vertexCount() const;
    std::vector<int> vertexSet() const;
    int& parent(int v);
    int parent(int v) const;
    int& edgeLevel(int v);
    int edgeLevel(int v) const;
    int root() const;

    std::vector<std::vector<int>> getChildrenLists() const;
    std::vector<int> getLoads(const MLCCNetwork &network) const;
    double cost(const MLCCNetwork& mlcc_network) const;
    bool isValid(const MLCCNetwork& network) const;

private:
    unsigned _root;
    std::vector<int> _parents;
    std::vector<int> _edge_levels;
};

}
