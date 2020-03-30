#pragma once

#include <random>
#include <functional>

#include <generator.hpp>
#include <geometry/point.hpp>

namespace MLCMST {
namespace geometry {
namespace generation {

class RealPointGenerator : public Generator<Point<double> >
{

private:
    double from, to;

    std::default_random_engine random_generator;
    std::uniform_real_distribution<double> random_distribution;
    std::function<double(void)> draw_random_number;


public:
    RealPointGenerator(double from = -10.0, double to = 10.0);
    ~RealPointGenerator();
    
    Point<double> generate();
    
};

}
}
}
