#pragma once

#include <memory>

#include <generator.hpp>
#include <geometry/point.hpp>

namespace MLCMST::geometry::generation {

class IntPointGenerator : public Generator<Point>
{
public:
    IntPointGenerator(int from, int to);
    IntPointGenerator(int from, int to, long seed);
    explicit IntPointGenerator(std::unique_ptr< Generator<int> > int_generator);

    Point generate() override;

private:
    std::unique_ptr< Generator<int> > _int_generator;
};

}
