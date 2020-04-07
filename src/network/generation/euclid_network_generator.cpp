#include <network/generation/euclid_network_generator.hpp>

#include <cmath>
#include <set>

#include <geometry/generation/real_point_set_generator.hpp>
#include <geometry/util.hpp>

namespace MLCMST {
namespace network {
namespace generation {

EuclidNetworkGenerator::EuclidNetworkGenerator(int N)
    : EuclidNetworkGenerator(std::make_unique<geometry::generation::RealPointSetGenerator>(N))
{

}

EuclidNetworkGenerator::EuclidNetworkGenerator(
    std::unique_ptr< util::Generator<vector<Point<double>>> > point_set_generator
)
    : point_set_generator(std::move(point_set_generator))
{

}

EuclidNetworkGenerator::~EuclidNetworkGenerator()
{

}

Network EuclidNetworkGenerator::generate()
{
    vector<Point<double>> points = point_set_generator->generate();
    vector<vector<double>> distances = geometry::util::createDistanceMatrix(points);
    return Network(distances);
}

}
}
}
