#include <util/number/real_number_generator.hpp>

#include <cassert>

namespace MLCMST::util::number {

RealNumberGenerator::RealNumberGenerator(double from, double to, unsigned seed)
    : _from(from), _to(to)
{
    _random_generator = std::default_random_engine(seed);
    _random_distribution = std::uniform_real_distribution<double>(from, to);
    _draw_random_number = std::bind(_random_distribution, _random_generator );
}


RealNumberGenerator::~RealNumberGenerator() = default;

double RealNumberGenerator::generate()
{
    return _draw_random_number();
}

}
