#include <catch2/catch.hpp>

#include <cmath>

#include <network/network.hpp>
#include <geometry/generation/real_point_generator.hpp>
#include <network/generation/euclid_network_generator.hpp>


using namespace MLCMST::network;


TEST_CASE( "Euclidean distance network generation", "[network][generation]" )
{
    using MLCMST::geometry::generation::RealPointGenerator;

    auto costs_in_range = [] (const Network& n, double max_cost) -> bool {
        int size = n.vertexCount();
        for (int i=0; i<size; i++) {
            for (int j=0; j<size; j++) {
                double cost = n.edgeCost(i,j);
                if (cost > max_cost || cost < 0.) {
                    return false;
                }
            }
        }
        return true;
    };

    auto loops_free = [] (const Network& n) -> bool {
        int size = n.vertexCount();
        for (int i=0; i<size; i++) {
            if (n.edgeCost(i,i) != 0)
                return false;
        }
        return true;
    };

    double l = 0.0, r = 10.0;
    double max_cost = std::sqrt(200);
    int size = 5;

    Network network = generation::EuclidNetworkGenerator(size, std::make_unique<RealPointGenerator>(l ,r)).generate();

    REQUIRE(network.vertexCount() == size );
    REQUIRE( costs_in_range(network, max_cost) );
    REQUIRE( loops_free(network) );
}