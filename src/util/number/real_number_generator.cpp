#include <util/number/real_number_generator.hpp>

#include <cassert>

namespace MLCMST {
namespace util {
namespace number {

RealNumberGenerator::RealNumberGenerator(double from, int to)
    : from(from), to(to)
{
    assert(("Interval start should be smaller than end", from < to));

    random_generator = std::default_random_engine();
    random_distribution = std::uniform_real_distribution<double>(from, to);
    draw_random_number = std::bind( random_distribution, random_generator );
}


RealNumberGenerator::~RealNumberGenerator()
{

}

double RealNumberGenerator::generate()
{
    return draw_random_number();
}

}
}
}
