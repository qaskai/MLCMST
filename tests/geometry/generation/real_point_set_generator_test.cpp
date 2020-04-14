#include <catch2/catch.hpp>

#include <vector>

#include <geometry/generation/real_point_set_generator.hpp>

using std::vector;
using namespace MLCMST::geometry;


TEST_CASE( "Real point set generation", "[geometry][generation][point]" )
{
    vector<Point<double>> points;

    SECTION( "Default constructor arguments" ) {
        double l = -10.0, r = 10.0;
        points = generation::RealPointSetGenerator(10).generate();

        for (const auto& p : points) {
            REQUIRE( (p.x >= l && p.x <= r) );
            REQUIRE( (p.y >= l && p.y <= r) );
        }
    }
    SECTION( "Custom constructor range" ) {
        double l = -100.0, r = 100.0;
        points = generation::RealPointSetGenerator(10, l, r).generate();

        for (const auto& p : points) {
            REQUIRE( (p.x >= l && p.x <= r) );
            REQUIRE( (p.y >= l && p.y <= r) );
        }
    }
}