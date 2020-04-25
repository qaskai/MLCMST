#include <geometry/serialization/point_set_serialization.hpp>

namespace MLCMST::geometry::serialization {


PointSetSerializer::PointSetSerializer() : PointSetSerializer(std::make_shared<PointSerializer>())
{
}

PointSetSerializer::PointSetSerializer(std::shared_ptr< Serializer<Point> > point_serializer)
    : _point_serializer(point_serializer)
{
}

PointSetSerializer::~PointSetSerializer() = default;

void PointSetSerializer::serialize(const std::vector<Point>& points, std::ostream& stream)
{
    stream << points.size() << "\n";
    for (const Point& p : points) {
        _point_serializer->serialize(p, stream);
    }
}



PointSetDeserializer::PointSetDeserializer() : PointSetDeserializer(std::make_shared<PointDeserializer>())
{
}

PointSetDeserializer::PointSetDeserializer(std::shared_ptr< Deserializer<Point> > point_deserializer)
    : _point_deserializer(point_deserializer)
{
}

PointSetDeserializer::~PointSetDeserializer() = default;

std::vector<Point> PointSetDeserializer::deserialize(std::istream& stream)
{
    int size;
    stream >> size;
    std::vector<Point> points;
    points.reserve(size);
    for (int i=0; i<size; i++) {
        Point p = _point_deserializer->deserialize(stream);
        points.push_back(p);
    }
    return points;
}

}
