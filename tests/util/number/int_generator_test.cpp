#include <catch2/catch.hpp>

#include <util/number/int_generator.hpp>

using namespace MLCMST::util::number;

TEST_CASE( "Random integer generator", "[generation][number]" )
{
    int l = -10, r = 10;
    int random_int = IntGenerator(l, r).generate();
    REQUIRE( ( random_int >= l  && random_int <= r ) );
}