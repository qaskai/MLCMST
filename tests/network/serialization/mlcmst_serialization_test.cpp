#include <catch2/catch.hpp>

#include <string>
#include <sstream>

#include <network/serialization/mlcst_serialization.hpp>
#include <util/util.hpp>

TEST_CASE( "MLCMST serialization", "[network][serialization]" )
{
    using MLCMST::network::MLCST;

    const int N = 5, root = 1;
    MLCST mlcmst(N, root);
    std::vector<int> nodes = ::MLCMST::util::firstN(N);

    std::vector<int> parents = {1, 1, 4, 2, 1};
    std::vector<int> levels = {0, 0, 1, 2, 3};
    for (int i : mlcmst.vertexSet()) {
        mlcmst.parent(i) = parents[i];
        mlcmst.facilityLevel(i) = levels[i];
    }

    std::string serialized_mlcmst =
        "5\n"
        "1\n"
        "1 1 4 2 1 \n"
        "0 0 1 2 3 \n";
    std::stringstream ss;

    SECTION( "serialize" ) {
        ::MLCMST::network::serialization::MLCSTSerializer serializer;

        serializer.serialize(mlcmst, ss);

        REQUIRE( ::MLCMST::util::read_stream(ss) == serialized_mlcmst );
    }
    SECTION( "deserialize" ) {
        ::MLCMST::network::serialization::MLCMSTDeserializer deserializer;
        ss << serialized_mlcmst;

        MLCST deserialized_mlcmst = deserializer.deserialize(ss);

        REQUIRE( deserialized_mlcmst.vertexCount() == N );
        REQUIRE( deserialized_mlcmst.root() == root );
        REQUIRE( deserialized_mlcmst.parents() == mlcmst.parents() );
        REQUIRE(deserialized_mlcmst.facilityLevels() == mlcmst.facilityLevels() );
    }
}