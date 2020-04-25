#pragma once

#include <geometry/point.hpp>
#include <serializer.hpp>
#include <deserializer.hpp>

namespace MLCMST::geometry::serialization {

class PointSerializer final : public Serializer<Point>
{
public:
    PointSerializer();
    ~PointSerializer() override;

    void serialize(const Point& p, std::ostream& stream) override;
};


class PointDeserializer final : public Deserializer<Point>
{
public:
    PointDeserializer();
    ~PointDeserializer() override;
    
    Point deserialize(std::istream& stream) override;
};

}
