#pragma once

#include <vector>

namespace MLCMST {
namespace network {

class Network
{
protected:
    std::vector<std::vector<double>> costs;

public:
    Network(const std::vector<std::vector<double>>& costs);
    virtual ~Network();

    double& edgeCost(int v, int w);
    const double& edgeCost(int v, int w) const;

};

}
}
