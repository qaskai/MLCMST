#pragma once

#include <memory>

#include <generator.hpp>
#include <geometry/point.hpp>

namespace MLCMST::geometry::generation {

class RealPointGenerator final : public Generator<Point<double> >
{

public:
    RealPointGenerator(double from = -10.0, double to = 10.0);
    RealPointGenerator(std::unique_ptr< Generator<double> > number_generator);
    ~RealPointGenerator() override;

    Point<double> generate() override;

private:
    std::unique_ptr< Generator<double> > _number_generator;

};

}
