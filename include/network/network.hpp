#pragma once

#include <vector>

namespace MLCMST::network {

class Network final
{
public:
    Network(const std::vector<std::vector<double>>& costs);
    ~Network();

    int vertexCount() const;
    const std::vector<std::vector<double>>& costs() const;
    double& edgeCost(int v, int w);
    const double& edgeCost(int v, int w) const;

private:
    std::vector<std::vector<double>> _costs;

};

bool operator==(const Network& n1, const Network& n2);
bool operator!=(const Network& n1, const Network& n2);

}
