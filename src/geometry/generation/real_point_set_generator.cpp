#include <geometry/generation/real_point_set_generator.hpp>

#include <set>

#include <geometry/generation/real_point_generator.hpp>

namespace MLCMST {
namespace geometry {
namespace generation {

RealPointSetGenerator::RealPointSetGenerator(int N, double from, double to)
    : RealPointSetGenerator(N, std::make_unique<RealPointGenerator>(from, to))
{

}

RealPointSetGenerator::RealPointSetGenerator(
    int N, std::unique_ptr<Generator<Point<double>>> point_generator
)
    : _size(N), _point_generator(std::move(point_generator))
{

}

RealPointSetGenerator::~RealPointSetGenerator() = default;

std::vector<Point<double>> RealPointSetGenerator::generate()
{
    auto cmp = [] (const Point<double>& p, const Point<double>& q) -> bool {
        if (p.x == q.x) {
            return p.y < q.y;
        } else {
            return p.x < q.x;
        }
    };
    std::set<Point<double>, decltype(cmp)> points(cmp);

    while (points.size() < _size) {
        points.insert(_point_generator->generate());
    }

    return std::vector<Point<double>>(points.begin(), points.end());
}

}
}
}
