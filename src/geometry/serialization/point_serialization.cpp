#include <geometry/serialization/point_serialization.hpp>


namespace MLCMST::geometry::serialization {

PointSerializer::PointSerializer() = default;

PointSerializer::~PointSerializer() = default;

void PointSerializer::serialize(const Point &p, std::ostream &stream) {
    stream << p.x << " " << p.y << "\n";
}



PointDeserializer::PointDeserializer()  = default;

PointDeserializer::~PointDeserializer() = default;

Point PointDeserializer::deserialize(std::istream &stream) {
    Point p;
    stream >> p.x >> p.y;
    return p;
}


}