#pragma once

#include <memory>
#include <vector>

#include <geometry/point.hpp>
#include <network/network.hpp>
#include <network/ml_centralized_capacitated_network.hpp>
#include <network/generation/level.hpp>
#include <util/generator.hpp>

namespace MLCMST {
namespace network {
namespace generation {

using geometry::Point;
using util::Generator;

using std::vector;

class EuclidMLCentralizedCapacitatedNetworkGenerator : public Generator<MLCentralizedCapacitatedNetwork>
{
private:
    std::vector<Level> levels;
    // std::unique_ptr< Generator<std::vector<Point<double>>> > point_set_generator;
    std::unique_ptr< Generator<Network> > network_generator;

    vector<double> flatten(const vector<vector<double>>& v);
    vector<double> multiply(vector<double> v, double scalar);
    vector<vector<double>> break_up(int N, const vector<double>& v);

public:
    EuclidMLCentralizedCapacitatedNetworkGenerator(int N, const std::vector<Level>& levels);
    EuclidMLCentralizedCapacitatedNetworkGenerator(
        const std::vector<Level>& levels,
        std::unique_ptr< Generator<std::vector<Point<double>>> > point_set_generator
        );
    ~EuclidMLCentralizedCapacitatedNetworkGenerator();
    
    MLCentralizedCapacitatedNetwork generate();
};

}
}
}
