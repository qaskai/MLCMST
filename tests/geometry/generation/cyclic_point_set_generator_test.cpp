#include <catch2/catch.hpp>

#include <geometry/generation/cyclic_point_set_generator.hpp>

TEST_CASE( "Cyclic point set generator", "[geometry][point]" )
{
    using MLCMST::geometry::Point;

    std::vector<std::vector<Point>> point_sets = {
        { Point(1,2), Point(1.5, 2.2), Point(2.3, 10) },
        { Point(2, 1.2) },
        {}
    };
    MLCMST::geometry::generation::CyclicPointSetGenerator generator(point_sets);

    for (int i=0; i<10; i++) {
        REQUIRE( generator.generate() == point_sets[i % point_sets.size()] );
    }
}