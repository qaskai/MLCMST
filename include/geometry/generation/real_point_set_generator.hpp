#pragma once

#include <random>
#include <functional>
#include <memory>
#include <vector>

#include <generator.hpp>
#include <geometry/point.hpp>

namespace MLCMST::geometry::generation {

class RealPointSetGenerator final : public Generator<std::vector<Point<double>> >
{

public:
    RealPointSetGenerator(int N, double from = -10.0, double to = 10.0);
    RealPointSetGenerator(int N, std::unique_ptr<Generator<Point<double>>> point_generator);
    ~RealPointSetGenerator() override;

    std::vector<Point<double>> generate() override;

private:
    int _size;
    std::unique_ptr<Generator<Point<double>>> _point_generator;

};

}
