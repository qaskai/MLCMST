#include <catch2/catch.hpp>

#include <vector>

#include <util/util.hpp>

TEST_CASE( "break up vector", "[util]" )
{
    std::vector<int> vec{ 1,2,3,4,5,6 };

    SECTION( "divisible" ) {
        const int N = 3;
        std::vector<std::vector<int>> broken = MLCMST::util::break_up(N, vec);
        for (int i=0; i<vec.size(); i++) {
            REQUIRE( broken[i/N][i%N] == vec[i] );
        }
    }
    SECTION( "not divisible" ) {
        const int N =4;
        std::vector<std::vector<int>> broken = MLCMST::util::break_up(N, vec);
        for (int i=0; i<vec.size(); i++) {
            REQUIRE( broken[i/N][i%N] == vec[i] );
        }
    }
}

TEST_CASE( "valueToIndex", "[util]" )
{
    std::vector<int> v{ 3,2,6 };

    std::unordered_map<int,int> mapping = MLCMST::util::valueToIndex(v);

    REQUIRE( mapping == std::unordered_map<int,int>{
        std::make_pair(3, 0), std::make_pair(2, 1), std::make_pair(6, 2)
    });
}

TEST_CASE( "groupIndexesByValue", "[util]" )
{
    std::vector<int> v{ 1,2,3,4,1,2,3 };

    auto groups = MLCMST::util::groupIndexesByValue(v);

    REQUIRE( groups.size() == 4 );
    REQUIRE( groups[1] == std::vector<int>{ 0,4 } );
    REQUIRE( groups[2] == std::vector<int>{ 1,5 } );
    REQUIRE( groups[3] == std::vector<int>{ 2,6 } );
    REQUIRE( groups[4] == std::vector<int>{ 3 } );
}

TEST_CASE( "statistic utils", "[util]" )
{
    SECTION( "mean" ) {
        double avg = MLCMST::util::mean({ 4, 9, 11, 12, 17, 5, 8, 12, 14 });
        REQUIRE( avg == Approx(10.222).margin(0.001) );
    }
    SECTION( "stdev" ) {
        double stdev = MLCMST::util::stdev({ 4, 9, 11, 12, 17, 5, 8, 12, 14 });
        REQUIRE( stdev == Approx(3.94).margin(0.01) );
    }
}
