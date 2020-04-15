#pragma once

#include <vector>

namespace MLCMST {
namespace network {

class Network final
{
public:
    Network(const std::vector<std::vector<double>>& costs);
    ~Network();

    int size() const;
    const std::vector<std::vector<double>>& costs() const;
    double& edgeCost(int v, int w);
    const double& edgeCost(int v, int w) const;

    friend bool operator==(const Network&, const Network&);

private:
    std::vector<std::vector<double>> _costs;

};

}
}
