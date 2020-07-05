#pragma once

#include <vector>

#include <geometry/point.hpp>
#include <generator.hpp>

namespace MLCMST::geometry::generation {

class CyclicPointSetGenerator final : public Generator<std::vector<Point>>
{
public:
    explicit CyclicPointSetGenerator(std::vector<std::vector<Point>> point_sets_);
    ~CyclicPointSetGenerator() override;

    std::vector<Point> generate() override;

private:
    std::vector<std::vector<Point>> point_sets_;
    int set_idx_;
};

}
