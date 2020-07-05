#include <geometry/generation/cyclic_point_generator.hpp>

namespace MLCMST::geometry::generation {

CyclicPointGenerator::CyclicPointGenerator(std::vector<Point> points) : points_(std::move(points)), idx_(0)
{
}

CyclicPointGenerator::~CyclicPointGenerator() = default;

Point CyclicPointGenerator::generate()
{
    if (idx_ == points_.size()) {
        idx_ = 0;
    }
    return points_[idx_++];
}

}
