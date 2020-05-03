#include <catch2/catch.hpp>

#include <geometry/generation/real_point_generator.hpp>

using namespace MLCMST::geometry;

TEST_CASE( "Real point generation", "[geometry][generation][point]" )
{
    double l = -100.0, r = 100.0;
    Point p = generation::RealPointGenerator(l, r).generate();

    REQUIRE( (p.x >= l && p.x <= r) );
    REQUIRE( (p.y >= l && p.y <= r) );
}