#include <catch2/catch.hpp>

#include <vector>

#include <geometry/generation/real_point_generator.hpp>
#include <geometry/generation/point_set_generator.hpp>
#include <geometry/generation/cyclic_point_generator.hpp>

using std::vector;
using namespace MLCMST::geometry;
using namespace MLCMST::geometry::generation;

TEST_CASE( "Real point set generation", "[geometry][generation][point]" )
{
    vector<Point> points;
    double l = -100.0, r = 100.0;
    points = PointSetGenerator(10, std::make_unique<RealPointGenerator>(l, r)).generate();

    REQUIRE( points.size() == 10 );
    for (const auto& p : points) {
        REQUIRE( (p.x >= l && p.x <= r) );
        REQUIRE( (p.y >= l && p.y <= r) );
    }
}

TEST_CASE( "Real point set generation uniqueness", "[geometry][generation][point]" )
{
    std::vector<Point> expected_points {
        Point(2,2), Point(1,1), Point(3,3)
    };
    using MLCMST::geometry::generation::CyclicPointGenerator;

    auto generated_points = PointSetGenerator(
            expected_points.size(), std::make_unique<CyclicPointGenerator>(expected_points)).generate();

    REQUIRE( generated_points.size() == expected_points.size() );
    for (int i=0; i<generated_points.size(); i++) {
        REQUIRE( generated_points[i] == expected_points[i] );
    }
}