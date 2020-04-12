#include <catch2/catch.hpp>

#include <sstream>

#include <geometry/point.hpp>
#include <geometry/serialization/point_serialization.hpp>

using namespace MLCMST::geometry;


TEST_CASE( "Point serialization", "[geometry][serialization][point]")
{
    Point<int> p(1,2);
    std::stringstream ss;

    SECTION( "serialize" ) {
        serialization::PointSerializer<int>().serialize(p, ss);
        Point<int> q;
        ss >> q.x >> q.y;
        REQUIRE( p == q );
    }
    SECTION( "deserialize" ) {
        ss << "1 2";
        Point<int> q = serialization::PointDeserializer<int>().deserialize(ss);
        REQUIRE( q == p );
    }
}