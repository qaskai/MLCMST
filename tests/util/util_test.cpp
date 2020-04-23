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