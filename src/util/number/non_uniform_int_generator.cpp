#include <util/number/non_uniform_int_generator.hpp>

#include <algorithm>

#include <util/util.hpp>
#include <util/number/real_number_generator.hpp>

namespace MLCMST::util::number {

NonUniformIntGenerator::NonUniformIntGenerator(int base, std::vector<double> probabilities)
    : NonUniformIntGenerator(base, std::move(probabilities), clockMilliseconds())
{
}

NonUniformIntGenerator::NonUniformIntGenerator(int base, std::vector<double> probabilities, long seed)
    : base_(base), intervals_(std::move(probabilities)),
      number_generator_(std::make_unique<RealNumberGenerator>(0,1, seed))
{
    for (int i=1; i < intervals_.size(); i++) {
        intervals_[i] += intervals_[i-1];
    }
}

NonUniformIntGenerator::~NonUniformIntGenerator() = default;

int NonUniformIntGenerator::generate()
{
    double x = number_generator_->generate();
    auto it = std::lower_bound(intervals_.begin(), intervals_.end(), x);
    return (it - intervals_.begin()) + base_;
}

}
