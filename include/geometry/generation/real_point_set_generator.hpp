#pragma once

#include <random>
#include <functional>
#include <memory>
#include <vector>

#include <util/generator.hpp>
#include <geometry/point.hpp>

namespace MLCMST {
namespace geometry {
namespace generation {

class RealPointSetGenerator final : public util::Generator<std::vector<Point<double>> >
{

private:
    int N;
    std::unique_ptr<util::Generator<Point<double>>> point_generator;


public:
    RealPointSetGenerator(int N, double from = -10.0, double to = 10.0);
    RealPointSetGenerator(int N, std::unique_ptr<util::Generator<Point<double>>> point_generator);
    ~RealPointSetGenerator();
    
    std::vector<Point<double>> generate();
    
};

}
}
}
