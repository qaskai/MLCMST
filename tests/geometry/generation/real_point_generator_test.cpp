#include <catch2/catch.hpp>

#include <geometry/generation/real_point_generator.hpp>

using namespace MLCMST::geometry;

TEST_CASE( "Real point generation", "[geometry][generation][point]" )
{
    Point<double> p;
    SECTION( "Default constructor" ) {
        double l = -10.0, r = 10.0;
        p = generation::RealPointGenerator().generate();

        REQUIRE( (p.x >= l && p.x <= r) );
        REQUIRE( (p.y >= l && p.y <= r) );
    }
    SECTION( "Range constructor" ) {
        double l = -100.0, r = 100.0;
        p = generation::RealPointGenerator(l, r).generate();

        REQUIRE( (p.x >= l && p.x <= r) );
        REQUIRE( (p.y >= l && p.y <= r) );
    }
}