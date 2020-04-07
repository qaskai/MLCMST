#pragma once

#include <ostream>

namespace MLCMST {

template <typename T>
class Serializer
{
public:
    virtual ~Serializer() {}
    
    virtual void serialize(const T& object, std::ostream& stream) = 0;

};

}
