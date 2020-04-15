#pragma once

#include <random>
#include <functional>

#include <generator.hpp>

namespace MLCMST {
namespace util {
namespace number {

class RealNumberGenerator final : public Generator<double>
{
public:
    RealNumberGenerator(double from = 0.0, int to = 1.0);
    ~RealNumberGenerator() override;
    double generate() override;

private:
    double _from, _to;
    std::default_random_engine _random_generator;
    std::uniform_real_distribution<double> _random_distribution;
    std::function<double(void)> _draw_random_number;

};

}
}
}
