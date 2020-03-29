#pragma once

#include <random>
#include <functional>

#include <generation/generator.hpp>
#include <model/geometry/point.hpp>

namespace MLCMST {
namespace generation {
namespace geometry {

class RealPointGenerator : public Generator<model::geometry::Point<double> >
{

private:
    std::default_random_engine random_generator;
    std::uniform_real_distribution<double> random_distribution;
    std::function<double(void)> draw_random_number;


public:
    RealPointGenerator(double from, double to);
    ~RealPointGenerator();
    
    model::geometry::Point<double> generate();
    
};

}
}
}
