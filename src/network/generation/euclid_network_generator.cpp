#include <network/generation/euclid_network_generator.hpp>

#include <set>

#include <geometry/generation/real_point_generator.hpp>
#include <geometry/generation/real_point_set_generator.hpp>
#include <geometry/util.hpp>

namespace MLCMST::network::generation {

EuclidNetworkGenerator::EuclidNetworkGenerator(int N, double from, double to)
    : EuclidNetworkGenerator(N, std::make_unique<geometry::generation::RealPointGenerator>(from, to))
{

}

EuclidNetworkGenerator::EuclidNetworkGenerator(
    int N, std::unique_ptr< Generator<Point<double>> > point_generator
)
    : EuclidNetworkGenerator(
        std::make_unique<geometry::generation::RealPointSetGenerator>(N, std::move(point_generator)))
{

}

EuclidNetworkGenerator::EuclidNetworkGenerator(
    std::unique_ptr< Generator<vector<Point<double>>> > point_set_generator
)
    : _point_set_generator(std::move(point_set_generator))
{

}

EuclidNetworkGenerator::~EuclidNetworkGenerator() = default;

Network EuclidNetworkGenerator::generate()
{
    vector<Point<double>> points = _point_set_generator->generate();
    vector<vector<double>> distances = geometry::util::createDistanceMatrix(points);
    return Network(distances);
}

}
