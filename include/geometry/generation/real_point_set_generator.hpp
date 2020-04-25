#pragma once

#include <random>
#include <functional>
#include <memory>
#include <vector>

#include <generator.hpp>
#include <geometry/point.hpp>

namespace MLCMST::geometry::generation {

class RealPointSetGenerator final : public Generator<std::vector<Point> >
{

public:
    RealPointSetGenerator(int N, double from = -10.0, double to = 10.0);
    RealPointSetGenerator(int N, std::unique_ptr<Generator<Point>> point_generator);
    ~RealPointSetGenerator() override;

    std::vector<Point> generate() override;

private:
    std::vector<Point>::size_type _size;
    std::unique_ptr<Generator<Point>> _point_generator;

};

}
