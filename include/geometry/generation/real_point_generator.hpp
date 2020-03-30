#pragma once

#include <memory>

#include <util/generator.hpp>
#include <geometry/point.hpp>

namespace MLCMST {
namespace geometry {
namespace generation {

class RealPointGenerator : public util::Generator<Point<double> >
{

private:
    std::unique_ptr< util::Generator<double> > number_generator;


public:
    RealPointGenerator(double from = -10.0, double to = 10.0);
    RealPointGenerator(std::unique_ptr< util::Generator<double> > number_generator);
    ~RealPointGenerator();
    
    Point<double> generate();
    
};

}
}
}
