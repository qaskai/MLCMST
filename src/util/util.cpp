#include <util/util.hpp>

#include <chrono>
#include <numeric>

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

}
