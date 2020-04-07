#pragma once

#include <geometry/point.hpp>
#include <util/serializer.hpp>
#include <util/deserializer.hpp>

namespace MLCMST {
namespace geometry {
namespace serialization {

template<typename T>
class PointSerializer final : util::Serializer<Point<T>>
{
public:
    PointSerializer() = default;
    ~PointSerializer() = default;
    
    void serialize(const Point<T>& p, std::ostream& stream);
};


template<typename T>
class PointDeserializer final : util::Deserializer<Point<T>>
{
public:
    PointDeserializer() = default;
    ~PointDeserializer() = default;
    
    Point<T> deserialize(std::istream& stream);
};


// ************* PointSerializer implementation ************* //

template<typename T>
void PointSerializer<T>::serialize(const Point<T>& p, std::ostream& stream)
{
    stream << p.x << " " << p.y << std::endl;
}



// ************* PointDeserializer implementation ************* //

template<typename T>
Point<T> PointDeserializer<T>::deserialize(std::istream& stream)
{
    Point<T> p;
    stream >> p.x >> p.y;
    return p;
}

}
}
}
