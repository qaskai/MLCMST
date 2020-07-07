#pragma once

#include <random>
#include <functional>

#include <generator.hpp>

namespace MLCMST::util::number {

class IntGenerator final : public Generator<int>
{
public:
    IntGenerator(int from, int to);
    IntGenerator(int from, int to, long seed);
    ~IntGenerator() override;

    int generate() override;

private:
    int _from, _to;
    std::default_random_engine _random_generator;
    std::uniform_int_distribution<int> _random_distribution;
    std::function<int(void)> _draw_random_number;

};

}
