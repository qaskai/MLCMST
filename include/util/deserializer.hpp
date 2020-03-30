#pragma once

#include <istream>

namespace MLCMST {
namespace util {

template <typename T>
class Deserializer
{
public:
    virtual ~Deserializer() {}
    
    virtual T deserialize(std::istream& stream) = 0;

};

}
}
