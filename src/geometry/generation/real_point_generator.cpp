#include <geometry/generation/real_point_generator.hpp>

#include <cassert>
#include <functional>
#include <random>

namespace MLCMST {
namespace geometry {
namespace generation {

RealPointGenerator::RealPointGenerator(double from, double to)
    : from(from), to(to)
{
    assert(("Interval start should be smaller than end", from < to));

    random_generator = std::default_random_engine();
    random_distribution = std::uniform_real_distribution<double>(from, to);
    draw_random_number = std::bind( random_distribution, random_generator );
}

RealPointGenerator::~RealPointGenerator()
{

}

Point<double> RealPointGenerator::generate()
{
    return Point<double>( draw_random_number(), draw_random_number() );
}

}
}
}
