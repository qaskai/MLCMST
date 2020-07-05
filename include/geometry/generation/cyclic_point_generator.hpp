#pragma once

#include <vector>

#include <generator.hpp>
#include <geometry/point.hpp>

namespace MLCMST::geometry::generation {

class CyclicPointGenerator final : public Generator<Point>
{
public:
    explicit CyclicPointGenerator(std::vector<Point> points);
    ~CyclicPointGenerator() override;

    Point generate() override;

private:
    std::vector<Point> points_;
    int idx_;
};

}
