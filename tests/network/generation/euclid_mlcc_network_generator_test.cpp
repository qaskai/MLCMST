#include <catch2/catch.hpp>

#include <cmath>
#include <utility>
#include <vector>

#include <generator.hpp>
#include <network/mlcc_network.hpp>
#include <network/generation/euclid_mlcc_network_generator.hpp>

using namespace MLCMST;
using namespace MLCMST::network;
using namespace MLCMST::network::generation;

TEST_CASE( "EuclidMLCCNetworkGenerator general functionality", "[network][generation]" )
{
    typedef EuclidMLCCNetworkGenerator::Level Level;
    typedef EuclidMLCCNetworkGenerator::CenterPosition CenterPosition;

    CenterPosition center_position = CenterPosition::RANDOM;
    std::vector<Level> levels{
        Level{1, 1.0}, Level{2, 1.5}, Level{5, 4.0}
    };
    double from = 0.0, to = 2.0;
    int size = 5;
    double max_cost = std::sqrt(8);

    auto cost_in_range = [] (const MLCCNetwork& n, double max_cost) {
        int size = n.vertexCount();
        const Network& originalNetwork = n.network(0);
        for (int i=0; i<size; i++) {
            for (int j=0; j<size; j++) {
                double cost = originalNetwork.edgeCost(i,j);
                REQUIRE( cost >= 0. );
                REQUIRE( cost <= max_cost );
            }
        }
    };

    auto check_demands = [] (const MLCCNetwork& n) {
        for (int i=0; i< n.vertexCount(); i++) {
            if ( i == n.center() ) {
                REQUIRE( n.demand(i) == 0 );
            } else {
                REQUIRE( n.demand(i) == 1 );
            }
        }
    };

    auto check_capacities = [] (const MLCCNetwork& n, const std::vector<Level>& levels) {
        for (int i=0; i< n.levelsNumber(); i++) {
            REQUIRE( n.edgeCapacity(i) == levels[i].capacity );
        }
    };

    auto check_multiplier = [] (double scalar, const Network& original, const Network& copy) {
        int size = original.vertexCount();
        for (int i=0; i<size; i++) {
            for (int j=0; j<size; j++) {
                REQUIRE( original.edgeCost(i,j) * scalar == copy.edgeCost(i,j) );
            }
        }
    };

    MLCCNetwork network = EuclidMLCCNetworkGenerator(size, from, to, center_position, levels).generate();

    REQUIRE(network.vertexCount() == size );
    REQUIRE(network.center() < size);
    REQUIRE(network.levelsNumber() == levels.size() );
    cost_in_range(network, max_cost);
    check_demands(network);
    check_capacities(network, levels);
    for (int i=0; i< network.levelsNumber(); i++) {
        check_multiplier( 
            levels[i].cost_multiplier,
            network.network(0),
            network.network(i)
        );
    }
}


class SetPointGenerator : public Generator<Point>
{
public:
    explicit SetPointGenerator(std::vector<Point>  points) : _points(std::move(points)), _idx(0) {
    }

    Point generate() override {
        if (_idx == _points.size())
            _idx = 0;
        return _points[_idx++];
    }

private:
    int _idx;
    std::vector<Point> _points;
};

TEST_CASE( "EuclidMLCCNetworkGenerator center type ", "[network][generation]" )
{
    typedef EuclidMLCCNetworkGenerator::Level Level;
    typedef EuclidMLCCNetworkGenerator::CenterPosition CenterPosition;

    std::vector<Level> levels {
        Level{1, 1}
    };
    std::vector<Point> points {
        Point(1,3), Point(3,1), Point(5,3), Point(3,5), Point(3,3)
    };
    auto point_generator = std::make_unique<SetPointGenerator>(points);

    SECTION( "random" ) {
        EuclidMLCCNetworkGenerator generator (points.size(), CenterPosition::RANDOM, levels, std::move(point_generator));

        MLCCNetwork network = generator.generate();

        REQUIRE( network.center() < points.size() );
        REQUIRE( network.center() >= 0 );
    }
    SECTION( "center" ) {
        EuclidMLCCNetworkGenerator generator (points.size(), CenterPosition::CENTER, levels, std::move(point_generator));

        MLCCNetwork network = generator.generate();

        REQUIRE( network.center() == 4 );
    }
    SECTION( "corner" ) {
        EuclidMLCCNetworkGenerator generator (points.size(), CenterPosition::CORNER, levels, std::move(point_generator));

        MLCCNetwork network = generator.generate();

        REQUIRE( network.center() == 1 );
    }

}
