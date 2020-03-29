#include <iostream>

#include <generation/geometry/real_point_generator.hpp>

int main(int argc, char const *argv[])
{
    std::cout << "Hello world" << std::endl;
    MLCMST::generation::geometry::RealPointGenerator g(0, 10);
    auto p = g.generate();
    std::cout << p.x << " " << p.y;
    return 0;
}