#pragma once

#include <memory>

#include <generator.hpp>
#include <geometry/point.hpp>

namespace MLCMST {
namespace geometry {
namespace generation {

class RealPointGenerator final : public Generator<Point<double> >
{

private:
    std::unique_ptr< Generator<double> > number_generator;


public:
    RealPointGenerator(double from = -10.0, double to = 10.0);
    RealPointGenerator(std::unique_ptr< Generator<double> > number_generator);
    ~RealPointGenerator();
    
    Point<double> generate();
    
};

}
}
}
