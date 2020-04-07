#pragma once

#include <vector>
#include <memory>

#include <geometry/serialization/point_serialization.hpp>
#include <serializer.hpp>
#include <deserializer.hpp>

namespace MLCMST {
namespace geometry {
namespace serialization {

template<typename T>
class PointSetSerializer final : Serializer< std::vector<Point<T>> >
{
private:
    std::shared_ptr< Serializer<Point<T>> > point_serializer;

public:
    PointSetSerializer();
    PointSetSerializer(std::shared_ptr< Serializer<Point<T>> > point_serializer);
    ~PointSetSerializer() = default;
    
    void serialize(const std::vector<Point<T>>& points, std::ostream& stream);
};


template<typename T>
class PointSetDeserializer final : Deserializer< std::vector<Point<T>> >
{
private:
    std::shared_ptr< Deserializer<Point<T>> > point_deserializer;

public:
    PointSetDeserializer();
    PointSetDeserializer(std::shared_ptr< Deserializer<Point<T>> > point_deserializer);
    ~PointSetDeserializer() = default;
    
    std::vector<Point<T>> deserialize(std::istream& stream);
};


// ************* PointSetSerializer implementation ************* //

template<typename T>
PointSetSerializer<T>::PointSetSerializer()
    : PointSetSerializer(std::make_shared<PointSerializer<T>>())
{

}

template<typename T>
PointSetSerializer<T>::PointSetSerializer(
    std::shared_ptr< Serializer<Point<T>> > point_serializer
)
    : point_serializer(point_serializer)
{

}

template<typename T>
void PointSetSerializer<T>::serialize(const std::vector<Point<T>>& points, std::ostream& stream)
{
    stream << points.size() << std::endl;
    for (const Point<T>& p : points) {
        point_serializer->serialize(p);
    }
}



// ************* PointSetDeserializer implementation ************* //

template<typename T>
PointSetDeserializer<T>::PointSetDeserializer() 
    : PointSetDeserializer(std::make_shared<PointDeserializer<T>>())
{

}

template<typename T>
PointSetDeserializer<T>::PointSetDeserializer(
    std::shared_ptr< Deserializer<Point<T>> > point_deserializer
)
    : point_deserializer(point_deserializer)
{

}

template<typename T>
std::vector<Point<T>> PointSetDeserializer<T>::deserialize(std::istream& stream)
{
    int size;
    stream >> size;
    std::vector<Point<T>> points;
    points.reserve(size);
    for (int i=0; i<size; i++) {
        Point<T> p = point_deserializer->deserialize(stream);
        points.push_back(p);
    }
    return points;
}

}
}
}
