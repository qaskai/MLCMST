#include <geometry/point.hpp>

namespace MLCMST::geometry {

Point::Point() = default;

Point::Point(double x, double y) : x(x), y(y) {

}

Point::~Point() = default;

bool operator==(const Point &p, const Point &q) {
    return (p.x == q.x) && (p.y == q.y);
}

}
