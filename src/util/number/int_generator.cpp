#include <util/number/int_generator.hpp>

#include <cassert>
#include <chrono>

namespace MLCMST::util::number {

IntGenerator::IntGenerator(int from, int to) : IntGenerator(from, to,
        std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch().count())
{
}

IntGenerator::IntGenerator(int from, int to, long seed)
    : _from(from), _to(to)
{
    assert(("Interval start should be smaller than end", from < to));

    _random_generator = std::default_random_engine(seed);
    _random_distribution = std::uniform_int_distribution<int>(from, to);
    _draw_random_number = std::bind(_random_distribution, _random_generator );
}

IntGenerator::~IntGenerator() = default;

int IntGenerator::generate()
{
    return _draw_random_number();
}

}
