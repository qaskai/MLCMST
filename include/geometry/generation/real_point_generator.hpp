#pragma once

#include <memory>

#include <generator.hpp>
#include <geometry/point.hpp>

namespace MLCMST::geometry::generation {

class RealPointGenerator final : public Generator<Point>
{

public:
    RealPointGenerator(double from, double to);
    RealPointGenerator(std::unique_ptr< Generator<double> > number_generator);
    ~RealPointGenerator() override;

    Point generate() override;

private:
    std::unique_ptr< Generator<double> > _number_generator;

};

}
