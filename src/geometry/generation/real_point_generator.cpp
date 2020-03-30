#include <geometry/generation/real_point_generator.hpp>

#include <util/number/real_number_generator.hpp>

namespace MLCMST {
namespace geometry {
namespace generation {

RealPointGenerator::RealPointGenerator(double from, double to)
    : RealPointGenerator(std::make_unique<util::number::RealNumberGenerator>(from, to))
{
    
}

RealPointGenerator::RealPointGenerator(
    std::unique_ptr< util::Generator<double> > number_generator
    ) : number_generator(std::move(number_generator))
{

}

RealPointGenerator::~RealPointGenerator()
{

}

Point<double> RealPointGenerator::generate()
{
    return Point<double>( number_generator->generate(), number_generator->generate() );
}

}
}
}
