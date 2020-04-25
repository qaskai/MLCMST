#include <util/util.hpp>

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

}
