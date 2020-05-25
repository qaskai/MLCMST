#pragma once

#include <limits>
#include <vector>

namespace MLCMST::network {

class Network final
{
public:
    explicit Network(std::vector<std::vector<double>>  costs);
    ~Network();

    [[nodiscard]] int vertexCount() const;
    [[nodiscard]] const std::vector<std::vector<double>>& costs() const;
    double& edgeCost(int v, int w);
    [[nodiscard]] const double& edgeCost(int v, int w) const;

    [[nodiscard]] std::vector<int> neighbourhood(int v) const;
    [[nodiscard]] std::vector<std::vector<int>> neighbourhoodList() const;

    [[nodiscard]] static double infinity();

private:
    std::vector<std::vector<double>> _costs;

    static double INFINITY_;

};

bool operator==(const Network& n1, const Network& n2);
bool operator!=(const Network& n1, const Network& n2);

}
