#include <network/generation/euclid_mlcc_network_generator.hpp>

#include <stdexcept>
#include <algorithm>
#include <utility>

#include <geometry/generation/real_point_generator.hpp>
#include <geometry/generation/point_set_generator.hpp>
#include <geometry/util.hpp>
#include <network/capacitated_network.hpp>
#include <util/util.hpp>

namespace MLCMST::network::generation {

EuclidMLCCNetworkGenerator::EuclidMLCCNetworkGenerator(
    int N, CenterPosition center_position, DemandType demand_type, const std::vector<Level>& levels,
    std::unique_ptr< Generator<Point> > point_generator
)
    : EuclidMLCCNetworkGenerator(
        N, center_position, demand_type, levels,
        std::make_unique<geometry::generation::PointSetGenerator>(N, std::move(point_generator))
    )
{

}

EuclidMLCCNetworkGenerator::EuclidMLCCNetworkGenerator(
    int N,
    CenterPosition center_position,
    DemandType demand_type,
    std::vector<Level> levels,
    std::unique_ptr< Generator<std::vector<Point>> > point_set_generator
)
    : _size(N), _center_position(center_position), _demand_type(demand_type), _levels(std::move(levels)),
      _point_set_generator(std::move(point_set_generator)),
      _demand_int_generator(util::number::IntGenerator(1,1)),
      _random_vertex_generator(util::number::IntGenerator(0, N - 1))
{
    _demands = std::vector<int>(_size, 1);
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
    vector<int> demands = generateDemands();
    demands[center] = 0;
    return MLCCNetwork(center, network_levels, demands);
}

int EuclidMLCCNetworkGenerator::determineCenter(const std::vector<Point>& points)
{
    auto distance_matrix = geometry::util::createDistanceMatrix(points);

    switch (_center_position) {
        case CenterPosition::RANDOM:
            return _random_vertex_generator.generate();
        case CenterPosition::CORNER:
        {
            auto it = std::min_element(points.begin(), points.end(), [] (const Point& p, const Point& q) -> bool {
                return (p.x != q.x) ? p.x < q.x : p.y < q.y;
            });
            return std::distance(points.begin(), it);
        }
        case CenterPosition::CENTER:
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

std::vector<int> EuclidMLCCNetworkGenerator::generateDemands()
{
    switch (_demand_type) {
        case DemandType::UNIT:
            return std::vector<int>(_size, 1);
        case DemandType::RANDOM:
        {
            std::vector<int> dem(_size, 0);
            for (int& x : dem) {
                x = _demand_int_generator.generate();
            }
            return dem;
        }
        case DemandType::SET:
            return _demands;
        default:
            throw std::invalid_argument("Unexpected demand type");
    }
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

void EuclidMLCCNetworkGenerator::setDemands(const std::vector<int>& demands)
{
    if (_size == demands.size()) {
        _demands = demands;
    }
}

void EuclidMLCCNetworkGenerator::setMaxRandomDemand(int max_demand)
{
    max_demand = std::min((unsigned) max_demand, _levels.back().capacity);
    _demand_int_generator = util::number::IntGenerator(1, max_demand);
}

}
