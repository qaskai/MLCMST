#include <util/number/int_generator.hpp>

#include <cassert>

namespace MLCMST {
namespace util {
namespace number {

IntGenerator::IntGenerator(int from, int to)
    : from(from), to(to)
{
    assert(("Interval start should be smaller than end", from < to));

    random_generator = std::default_random_engine();
    random_distribution = std::uniform_int_distribution<int>(from, to);
    draw_random_number = std::bind( random_distribution, random_generator );
}

IntGenerator::~IntGenerator()
{

}

int IntGenerator::generate()
{
    return draw_random_number();
}

}
}
}
