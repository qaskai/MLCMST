#include <catch2/catch.hpp>

#include <sstream>
#include <vector>

#include <geometry/point.hpp>
#include <geometry/serialization/point_set_serialization.hpp>
#include <util/util.hpp>

using namespace MLCMST;
using namespace MLCMST::geometry;


TEST_CASE( "Point set serialization", "[geometry][serialization][point]" )
{
    auto points = std::vector<Point> {
        Point(1,2), Point(2,3), Point(3,4)
    };
    std::string serialized_points =
        "3\n"
        "1 2\n"
        "2 3\n"
        "3 4\n";

    std::stringstream ss;

    SECTION( "serialize" ) {
        serialization::PointSetSerializer().serialize(points, ss);

        REQUIRE( util::read_stream(ss) == serialized_points );
    }
    SECTION( "deserialize" ) {
        ss << serialized_points;
        auto deserialized_points =
            serialization::PointSetDeserializer().deserialize(ss);

        REQUIRE( deserialized_points.size() == points.size() );
        for (int i=0; i<points.size(); i++) {
            REQUIRE( deserialized_points[i] == points[i] );
        }
    }
}
