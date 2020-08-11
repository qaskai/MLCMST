#pragma once

#include <vector>

#include <network/mlcc_network.hpp>

namespace MLCMST::network {

/**
 * MLCMST data class.
 *
 * Assumes that parents vector forms a directed tree and its root (and only the root) has a parent set to itself.
 */
class MLCST final
{
public:
    MLCST(int N, int root);
    ~MLCST();

    /**
     * Sets minimal levels on edges that still make it viable.
     * @return False if viable setting of the levels was not possible.
     */
    bool setMinimalViableLevels(const MLCCNetwork& mlcc_network);

    [[nodiscard]] unsigned vertexCount() const;
    [[nodiscard]] std::vector<int> vertexSet() const;
    int& parent(int v);
    [[nodiscard]] int parent(int v) const;
    [[nodiscard]] const std::vector<int>& parents() const;
    int& facilityLevel(int v);
    [[nodiscard]] int facilityLevel(int v) const;
    [[nodiscard]] const std::vector<int>& facilityLevels() const;
    [[nodiscard]] int root() const;

    [[nodiscard]] std::vector<std::vector<int>> childrenLists() const;
    [[nodiscard]] std::vector<int> leafs() const;
    [[nodiscard]] std::vector<int> nonLeafTerminals() const;
    [[nodiscard]] std::vector<int> subtreeVertices(int v) const;
    [[nodiscard]] std::vector<int> pathToRoot(int v) const;

    [[nodiscard]] double cost(const MLCCNetwork& mlcc_network) const;
    [[nodiscard]] bool checkFeasibility(const MLCCNetwork& network) const;
    [[nodiscard]] std::vector<int> loads(const MLCCNetwork &network) const;
    [[nodiscard]] std::vector<int> slack(const MLCCNetwork& network) const;
    [[nodiscard]] std::vector<int> reserves(const MLCCNetwork &network) const;
    [[nodiscard]] std::vector<int> subnet() const;

    static MLCST star(const MLCCNetwork& network);

private:
    int _root;
    std::vector<int> _parents;
    std::vector<int> _facility_levels;
};

}
