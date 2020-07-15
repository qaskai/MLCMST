#include <catch2/catch.hpp>

#include <util/number/non_uniform_int_generator.hpp>

#include <map>

TEST_CASE( "Non-uniform Int generator", "[generation][number]" )
{
    using MLCMST::util::number::NonUniformIntGenerator;
    int base = 10;
    int iterations = 1000;
    std::vector<double> probabilities { 0.2, 0.1, 0.3, 0.4 };
    NonUniformIntGenerator generator(base, probabilities, 1);

    std::map<int,int> counter;
    for (int i=0; i<iterations; i++) {
        double x = generator.generate();
        counter[x]++;
    }
    REQUIRE( counter.size() == probabilities.size() );
    for (int i=0; i < probabilities.size(); i++) {
        double percentage = (double) counter[base + i] / iterations;
        REQUIRE( Approx(percentage).margin(0.02) == probabilities[i] );
    }
}

TEST_CASE( "Non-uniform integer generator - invalid probabilities", "[generation][number]")
{
    using MLCMST::util::number::NonUniformIntGenerator;
    SECTION( "does not add up to 1" ) {
        REQUIRE_THROWS_AS(NonUniformIntGenerator(1, {0.5,0.3}), std::invalid_argument);
    }
    SECTION( "probability lower than 0" ) {
        REQUIRE_THROWS_AS(NonUniformIntGenerator(1, {0.5, -0.5, 1.0}), std::invalid_argument);
    }
    SECTION( "probability hier then 1" ) {
        REQUIRE_THROWS_AS(NonUniformIntGenerator(1, {1.5}), std::invalid_argument);
    }
}