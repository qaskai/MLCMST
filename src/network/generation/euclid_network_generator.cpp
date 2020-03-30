#include <network/generation/euclid_network_generator.hpp>

#include <cmath>
#include <set>

#include <geometry/generation/real_point_generator.hpp>

namespace MLCMST {
namespace network {
namespace generation {

EuclidNetworkGenerator::EuclidNetworkGenerator(int N)
    : EuclidNetworkGenerator(N, std::make_unique<geometry::generation::RealPointGenerator>())
{

}

EuclidNetworkGenerator::EuclidNetworkGenerator(
    int N, unique_ptr<Generator<Point<double>>> point_generator
    ) : N(N), point_generator(std::move(point_generator))
{

}

EuclidNetworkGenerator::~EuclidNetworkGenerator()
{

}

Network EuclidNetworkGenerator::generate()
{
    vector<Point<double>> points = generatePoints(N);
    vector<vector<double>> distances = createDistanceMatrix(points);
    return Network(distances);
}


double EuclidNetworkGenerator::eulcidDistance(const Point<double>& p, const Point<double>& q)
{
    double a = p.x - q.x;
    double b = p.y - q.y;
    return sqrt(a*a + b*b);
}

vector<Point<double>> EuclidNetworkGenerator::generatePoints(int N)
{
    auto cmp = [] (const Point<double>& p, const Point<double>& q) -> bool {
        if (p.x == q.x) {
            return p.y < q.y;
        } else {
            return p.x < q.x;
        }
    };
    std::set<Point<double>, decltype(cmp)> points(cmp);

    while (points.size() < N) {
        points.insert(point_generator->generate());
    }

    return vector<Point<double>>(points.begin(), points.end());
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
