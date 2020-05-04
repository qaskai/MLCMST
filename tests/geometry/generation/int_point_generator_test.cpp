#include <catch2/catch.hpp>

#include <cmath>

#include <geometry/generation/int_point_generator.hpp>

using namespace MLCMST::geometry;

TEST_CASE( "Int point generation", "[geometry][generation][point]" )
{
    int l = -10, r = 10;
    Point p = generation::IntPointGenerator(l, r).generate();

    REQUIRE( std::abs(std::round(p.x) - p.x) == Approx(0).margin(0.00001) );
    REQUIRE( std::abs(std::round(p.y) - p.y) == Approx(0).margin(0.00001) );
    REQUIRE( (p.x >= l && p.x <= r) );
    REQUIRE( (p.y >= l && p.y <= r) );
}
