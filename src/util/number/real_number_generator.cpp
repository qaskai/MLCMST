#include <util/number/real_number_generator.hpp>

#include <cassert>

namespace MLCMST {
namespace util {
namespace number {

RealNumberGenerator::RealNumberGenerator(double from, int to)
    : _from(from), _to(to)
{
    assert(("Interval start should be smaller than end", from < to));

    _random_generator = std::default_random_engine();
    _random_distribution = std::uniform_real_distribution<double>(from, to);
    _draw_random_number = std::bind(_random_distribution, _random_generator );
}


RealNumberGenerator::~RealNumberGenerator() = default;

double RealNumberGenerator::generate()
{
    return _draw_random_number();
}

}
}
}
