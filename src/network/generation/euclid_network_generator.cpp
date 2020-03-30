#include <network/generation/euclid_network_generator.hpp>

#include <cmath>
#include <set>

#include <geometry/generation/real_point_set_generator.hpp>

namespace MLCMST {
namespace network {
namespace generation {

EuclidNetworkGenerator::EuclidNetworkGenerator(int N)
    : EuclidNetworkGenerator(std::make_unique<geometry::generation::RealPointSetGenerator>(N))
{

}

EuclidNetworkGenerator::EuclidNetworkGenerator(
    std::unique_ptr< util::Generator<vector<Point<double>>> > point_set_generator
    ) : point_set_generator(std::move(point_set_generator))
{

}

EuclidNetworkGenerator::~EuclidNetworkGenerator()
{

}

Network EuclidNetworkGenerator::generate()
{
    vector<Point<double>> points = point_set_generator->generate();
    vector<vector<double>> distances = createDistanceMatrix(points);
    return Network(distances);
}


double EuclidNetworkGenerator::eulcidDistance(const Point<double>& p, const Point<double>& q)
{
    double a = p.x - q.x;
    double b = p.y - q.y;
    return sqrt(a*a + b*b);
}

vector<vector<double>> EuclidNetworkGenerator::createDistanceMatrix(const vector<Point<double>>& points)
{
    int N = points.size();
    vector<vector<double>> distances(N, vector<double>(N));
    for (int i=0; i<N; i++) {
        for (int j=0; j<N; j++) {
            distances[i][j] = eulcidDistance(points[i], points[j]);
        }
    }
    return distances;
}

}
}
}
