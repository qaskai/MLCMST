#include <util/number/non_uniform_int_generator.hpp>

#include <algorithm>
#include <cmath>

#include <util/util.hpp>
#include <util/number/real_number_generator.hpp>

namespace MLCMST::util::number {

double NonUniformIntGenerator::EPS_ = 1e-6;

NonUniformIntGenerator::NonUniformIntGenerator(int base, const std::vector<double>& probabilities)
    : NonUniformIntGenerator(base, probabilities, clockMilliseconds())
{
}

NonUniformIntGenerator::NonUniformIntGenerator(int base, const std::vector<double>& probabilities, long seed)
    : base_(base), number_generator_(std::make_unique<RealNumberGenerator>(0,1, seed))
{
    intervals_ = probabilities;
    for (int i=1; i < intervals_.size(); i++) {
        if ( probabilities[i] < 0 || probabilities[i] > 1.0 + EPS_ ) {
            throw std::invalid_argument("All probabilities must have a value between 0 and 1");
        }
        intervals_[i] += intervals_[i-1];
    }
    if (std::abs( intervals_.back() - 1.0 ) > 1e-6) {
        throw std::invalid_argument("Probabilities do not sum up to 1");
    }
    for (double& x : intervals_) {
        x = std::max(0.0, std::min(1.0, x));
    }
}

NonUniformIntGenerator::~NonUniformIntGenerator() = default;

int NonUniformIntGenerator::generate()
{
    double x = number_generator_->generate();
    auto it = std::lower_bound(intervals_.begin(), intervals_.end(), x);
    if (std::abs(*it) < EPS_) {
        it = std::upper_bound(intervals_.begin(), intervals_.end(), x);
    }
    return (it - intervals_.begin()) + base_;
}

}
