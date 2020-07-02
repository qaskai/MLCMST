#include <network/generation/euclid_mlcc_network_generator.hpp>

#include <algorithm>
#include <utility>

#include <geometry/generation/real_point_generator.hpp>
#include <geometry/generation/point_set_generator.hpp>
#include <geometry/util.hpp>
#include <network/capacitated_network.hpp>
#include <util/util.hpp>

namespace MLCMST::network::generation {

EuclidMLCCNetworkGenerator::EuclidMLCCNetworkGenerator(
    unsigned N, CenterPosition center_position, const std::vector<Level>& levels,
    std::unique_ptr< Generator<Point> > point_generator
)
    : EuclidMLCCNetworkGenerator(
        N, center_position, levels,
        std::make_unique<geometry::generation::PointSetGenerator>(N, std::move(point_generator))
    )
{

}

EuclidMLCCNetworkGenerator::EuclidMLCCNetworkGenerator(
    unsigned N,
    CenterPosition center_position,
    std::vector<Level> levels,
    std::unique_ptr< Generator<std::vector<Point>> > point_set_generator
)
    : _center_position(center_position), _levels(std::move(levels)), _point_set_generator(std::move(point_set_generator)),
        _int_generator(util::number::IntGenerator(0, static_cast<int>(N-1)))
{
}

EuclidMLCCNetworkGenerator::~EuclidMLCCNetworkGenerator() = default;

MLCCNetwork EuclidMLCCNetworkGenerator::generate()
{
    vector<Point> points = _last_point_set = _point_set_generator->generate();
    Network network = Network(geometry::util::createDistanceMatrix(points));
    int N = network.vertexCount();
    vector<CapacitatedNetwork> network_levels;
    for (Level level : _levels) {
        auto new_costs = util::break_up(N, multiply(flatten(network.costs()), level.cost_multiplier));
        network_levels.emplace_back(level.capacity, Network(new_costs));
    }
    const int center = determineCenter(points);
    vector<int> demands = vector<int>(N, 1);
    demands[center] = 0;
    return MLCCNetwork(center, network_levels, demands);
}

int EuclidMLCCNetworkGenerator::determineCenter(const std::vector<Point>& points)
{
    auto distance_matrix = geometry::util::createDistanceMatrix(points);

    switch (_center_position) {
        case RANDOM:
            return _int_generator.generate();
        case CORNER:
        {
            auto it = std::min_element(points.begin(), points.end(), [] (const Point& p, const Point& q) -> bool {
                return (p.y != q.y) ? p.y < q.y : p.x < q.x;
            });
            return std::distance(points.begin(), it);
        }
        case CENTER:
        {
            std::vector<double> max_distances;
            std::transform(distance_matrix.begin(), distance_matrix.end(), std::back_inserter(max_distances),
                    [] (const std::vector<double>& v) -> double { return *std::max_element(v.begin(), v.end()); });
            auto it = std::min_element(max_distances.begin(), max_distances.end());
            return std::distance(max_distances.begin(), it);
        }
    }
    return 0;
}

vector<double> EuclidMLCCNetworkGenerator::flatten(const vector<vector<double>>& v)
{
    vector<double> result;
    for (const vector<double>& inV : v) {
        for (double x : inV) {
            result.push_back(x);
        }
    }
    return result;
}

vector<double> EuclidMLCCNetworkGenerator::multiply(vector<double> v, double scalar)
{
    for (double& x : v) {
        x *= scalar;
    }
    return v;
}

std::vector<Point> EuclidMLCCNetworkGenerator::lastPointSet() const
{
    return _last_point_set;
}

}
