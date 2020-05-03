#include <geometry/generation/real_point_set_generator.hpp>

#include <set>

#include <geometry/generation/real_point_generator.hpp>

namespace MLCMST::geometry::generation {

RealPointSetGenerator::RealPointSetGenerator(int N, double from, double to)
    : RealPointSetGenerator(N, std::make_unique<RealPointGenerator>(from, to))
{

}

RealPointSetGenerator::RealPointSetGenerator(int N, std::unique_ptr<Generator<Point>> point_generator)
    : _size(N), _point_generator(std::move(point_generator))
{

}

RealPointSetGenerator::~RealPointSetGenerator() = default;

std::vector<Point> RealPointSetGenerator::generate()
{
    auto cmp = [] (const std::pair<Point, int>& first, const std::pair<Point, int>& second) -> bool {
        Point p = first.first;
        Point q = second.first;
        return (p.x != q.x) ? p.x < q.x : p.y < q.y;
    };
    std::set<std::pair<Point, int>, decltype(cmp)> points(cmp);

    while (points.size() < _size) {
        int idx = points.size();
        points.insert(std::make_pair(_point_generator->generate(), idx));
    }

    std::vector<Point> ordered_points(_size);
    for (auto [p, id] : points) {
        ordered_points[id] = p;
    }
    return ordered_points;
}

}
