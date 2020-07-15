#include <util/util.hpp>

#include <chrono>
#include <numeric>
#include <cmath>

namespace MLCMST::util {

std::string read_stream(std::istream& stream)
{
    return std::string(std::istreambuf_iterator<char>(stream), {});
}

std::vector<int> firstN(unsigned int N) {
    std::vector<int> v(N);
    std::iota(v.begin(), v.end(), 0);
    return v;
}

long clockMilliseconds()
{
    auto now = std::chrono::system_clock::now();
    auto time_point = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    return time_point.time_since_epoch().count();
}

double mean(const std::vector<double> &v)
{
    double sum = std::accumulate(v.begin(), v.end(), 0.0);
    return sum / v.size();
}

double stdev(const std::vector<double>& v)
{
    double mean = util::mean(v);
    std::vector<double> diff(v.size());
    std::transform(v.begin(), v.end(), diff.begin(), [mean](double x) { return x - mean; });
    double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
    return std::sqrt(sq_sum / v.size());
}

}
