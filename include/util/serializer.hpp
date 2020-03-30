#pragma once

#include <ostream>

namespace MLCMST {
namespace util {

template <typename T>
class Serializer
{
public:
    virtual ~Serializer() {}
    
    virtual void serialize(const T& object, std::ostream& stream) = 0;

};

}
}
