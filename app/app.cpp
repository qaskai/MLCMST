#include <iostream>
#include <vector>

#include <geometry/generation/real_point_generator.hpp>
#include <network/capacitated_network.hpp>

int main(int argc, char const *argv[])
{
    std::cout << "Hello world" << std::endl;
    MLCMST::geometry::generation::RealPointGenerator g(0, 10);
    auto p = g.generate();
    std::cout << p.x << " " << p.y << std::endl;

    MLCMST::network::CapacitatedNetwork qq({ {1.0, 2.3}, {2.3, 1.2}}, 3);
    std::cout << qq.edgeCost(0,1) << std::endl;
    return 0;
}