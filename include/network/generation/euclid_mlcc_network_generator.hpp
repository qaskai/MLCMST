#pragma once

#include <memory>
#include <vector>

#include <geometry/point.hpp>
#include <network/network.hpp>
#include <network/mlcc_network.hpp>
#include <network/generation/level.hpp>
#include <util/generator.hpp>

namespace MLCMST {
namespace network {
namespace generation {

using geometry::Point;
using util::Generator;

using std::vector;

class EuclidMLCCNetworkGenerator : public Generator<MLCCNetwork>
{
private:
    std::vector<Level> levels;
    std::unique_ptr< Generator<Network> > network_generator;

    vector<double> flatten(const vector<vector<double>>& v);
    vector<double> multiply(vector<double> v, double scalar);
    vector<vector<double>> break_up(int N, const vector<double>& v);

public:
    EuclidMLCCNetworkGenerator(int N, const std::vector<Level>& levels);
    EuclidMLCCNetworkGenerator(
        const std::vector<Level>& levels,
        std::unique_ptr< Generator<std::vector<Point<double>>> > point_set_generator
    );
    ~EuclidMLCCNetworkGenerator();
    
    MLCCNetwork generate();
};

}
}
}
