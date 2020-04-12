#include <util/util.hpp>

namespace MLCMST {
namespace util {

std::string read_stream(std::istream& stream)
{
    return std::string(std::istreambuf_iterator<char>(stream), {});
}

}
}
