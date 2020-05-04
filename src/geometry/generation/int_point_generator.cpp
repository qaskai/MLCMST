#include <geometry/generation/int_point_generator.hpp>

#include <util/number/int_generator.hpp>

namespace MLCMST::geometry::generation {

IntPointGenerator::IntPointGenerator(int from, int to)
    : IntPointGenerator(std::make_unique<util::number::IntGenerator>(from, to))
{
}

IntPointGenerator::IntPointGenerator(std::unique_ptr<Generator<int>> int_generator)
    : _int_generator(std::move(int_generator))
{

}

Point IntPointGenerator::generate()
{
    return Point( _int_generator->generate(), _int_generator->generate() );
}


}