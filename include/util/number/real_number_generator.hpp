#pragma once

#include <random>
#include <functional>

#include <generator.hpp>

namespace MLCMST {
namespace util {
namespace number {

class RealNumberGenerator final : public Generator<double>
{

private:
    double from, to;

    std::default_random_engine random_generator;
    std::uniform_real_distribution<double> random_distribution;
    std::function<double(void)> draw_random_number;


public:
    RealNumberGenerator(double from = 0.0, int to = 1.0);
    ~RealNumberGenerator();
    
    double generate();
    
};

}
}
}
