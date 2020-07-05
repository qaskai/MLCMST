#include <catch2/catch.hpp>

#include <vector>

#include <geometry/generation/cyclic_point_generator.hpp>

TEST_CASE( "Cyclic point generator", "[geometry][point]" )
{
    using MLCMST::geometry::Point;

    std::vector<Point> points = {
        Point(1,2), Point(1.2,3.1), Point(2, 1.2), Point(3.1, 2)
    };
    MLCMST::geometry::generation::CyclicPointGenerator generator(points);

    for (int i=0; i<10; i++) {
        REQUIRE( generator.generate() == points[i % points.size()] );
    }
}