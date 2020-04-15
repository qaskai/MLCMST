#pragma once

#include <geometry/point.hpp>
#include <serializer.hpp>
#include <deserializer.hpp>

namespace MLCMST {
namespace geometry {
namespace serialization {

template<typename T>
class PointSerializer final : public Serializer<Point<T>>
{
public:
    PointSerializer() = default;
    ~PointSerializer() override = default;

    void serialize(const Point<T>& p, std::ostream& stream) override;
};


template<typename T>
class PointDeserializer final : public Deserializer<Point<T>>
{
public:
    PointDeserializer() = default;
    ~PointDeserializer() override = default;
    
    Point<T> deserialize(std::istream& stream) override;
};


// ************* PointSerializer implementation ************* //

template<typename T>
void PointSerializer<T>::serialize(const Point<T>& p, std::ostream& stream)
{
    stream << p.x << " " << p.y << "\n";
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
