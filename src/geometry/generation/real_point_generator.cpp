#include <geometry/generation/real_point_generator.hpp>

#include <util/number/real_number_generator.hpp>

namespace MLCMST::geometry::generation {

RealPointGenerator::RealPointGenerator(double from, double to)
    : RealPointGenerator(std::make_unique<util::number::RealNumberGenerator>(from, to))
{
    
}

RealPointGenerator::RealPointGenerator(std::unique_ptr< Generator<double> > number_generator)
    : _number_generator(std::move(number_generator))
{

}

RealPointGenerator::~RealPointGenerator() = default;

Point RealPointGenerator::generate()
{
    return Point(_number_generator->generate(), _number_generator->generate() );
}

}
