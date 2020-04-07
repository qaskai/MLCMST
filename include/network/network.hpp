#pragma once

#include <vector>

namespace MLCMST {
namespace network {

class Network final
{
private:
    std::vector<std::vector<double>> costs;

public:
    Network(const std::vector<std::vector<double>>& costs);
    virtual ~Network();

    int getSize() const;
    const std::vector<std::vector<double>>& getCosts() const;
    double& edgeCost(int v, int w);
    const double& edgeCost(int v, int w) const;

};

}
}
