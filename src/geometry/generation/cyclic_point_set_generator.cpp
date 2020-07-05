#include <geometry/generation/cyclic_point_set_generator.hpp>

namespace MLCMST::geometry::generation {


CyclicPointSetGenerator::CyclicPointSetGenerator(std::vector<std::vector<Point>> point_sets_)
    : point_sets_(std::move(point_sets_)), set_idx_(0)
{
}

CyclicPointSetGenerator::~CyclicPointSetGenerator() = default;

std::vector<Point> CyclicPointSetGenerator::generate()
{
    if (set_idx_ == point_sets_.size()) {
        set_idx_ = 0;
    }
    return point_sets_[set_idx_++];
}

}
