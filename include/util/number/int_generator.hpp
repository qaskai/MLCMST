#pragma once

#include <random>
#include <functional>

#include <util/generator.hpp>

namespace MLCMST {
namespace util {
namespace number {

class IntGenerator : public Generator<int>
{
private:
    int from, to;

    std::default_random_engine random_generator;
    std::uniform_int_distribution<int> random_distribution;
    std::function<int(void)> draw_random_number;

public:
    IntGenerator(int from, int to);
    ~IntGenerator();
    
    int generate();

};

}
}
}
