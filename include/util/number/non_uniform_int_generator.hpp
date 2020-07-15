#pragma once

#include <vector>
#include <memory>

#include <generator.hpp>


namespace MLCMST::util::number {

class NonUniformIntGenerator final : public Generator<int>
{
public:
    NonUniformIntGenerator(int base, const std::vector<double>& probabilities);
    NonUniformIntGenerator(int base, const std::vector<double>& probabilities, long seed);
    ~NonUniformIntGenerator() override;

    int generate() override;
private:
    static double EPS_;

    int base_;
    std::vector<double> intervals_;

    std::unique_ptr<Generator<double>> number_generator_;

};

}
