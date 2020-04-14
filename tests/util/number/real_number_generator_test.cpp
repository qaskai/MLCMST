#include <catch2/catch.hpp>

#include <util/number/real_number_generator.hpp>

using namespace MLCMST::util::number;

TEST_CASE( "Random real number generator", "[generation][number]" )
{
    double l = -10.0, r = 10.0;
    double random_number = RealNumberGenerator(l, r).generate();
    REQUIRE( ( random_number >= l  && random_number <= r ) );
}