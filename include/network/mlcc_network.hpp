#pragma once

#include <vector>

#include <network/capacitated_network.hpp>


namespace MLCMST::network {

/**
 * MLCC stands for Multi Level Centralized Capacitated
 * 
 */
class MLCCNetwork final
{
public:
    /**
     * MLCCNetwork constructor.
     * @param center - network center
     * @param networks - capacitated networks. Should be sorted increasingly by capacity and have unique capacity.
     * @param demands - vector of units vertices desire to send to the center (ie vertex weights)
     */
    MLCCNetwork(int center, std::vector<CapacitatedNetwork> networks, std::vector<int> demands);
    ~MLCCNetwork();

    [[nodiscard]] int center() const;
    [[nodiscard]] int vertexCount() const;
    [[nodiscard]] std::vector<int> vertexSet() const;
    [[nodiscard]] std::vector<int> terminalVertexSet() const;
    [[nodiscard]] int levelsNumber() const;
    [[nodiscard]] std::vector<int> levels() const;
    [[nodiscard]] int demand(int v) const;
    [[nodiscard]] const std::vector<int>& demands() const;
    [[nodiscard]] const Network& network(int i) const;
    [[nodiscard]] const std::vector<CapacitatedNetwork>& networks() const;
    [[nodiscard]] int edgeCapacity(int network_i) const;
    [[nodiscard]] double edgeCost(int v, int w, int level) const;
    [[nodiscard]] int maxEdgeCapacity() const;

    [[nodiscard]] MLCCNetwork multiplyEdgeCosts(double scalar) const;

    static double infinity();
    [[nodiscard]] std::pair<MLCCNetwork, std::vector<int>> subNetwork(std::vector<int> vertices) const;

private:
    int _center;
    std::vector<CapacitatedNetwork> _networks;
    std::vector<int> _demands;

};

bool operator==(const MLCCNetwork& n1, const MLCCNetwork& n2);
bool operator!=(const MLCCNetwork& n1, const MLCCNetwork& n2);

}
