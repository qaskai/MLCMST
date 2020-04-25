#pragma once

#include <vector>
#include <memory>

#include <geometry/serialization/point_serialization.hpp>
#include <serializer.hpp>
#include <deserializer.hpp>

namespace MLCMST::geometry::serialization {

class PointSetSerializer final : public Serializer< std::vector<Point> >
{
public:
    PointSetSerializer();
    PointSetSerializer(std::shared_ptr< Serializer<Point> > point_serializer);
    ~PointSetSerializer() override;

    void serialize(const std::vector<Point>& points, std::ostream& stream) override;

private:
    std::shared_ptr< Serializer<Point> > _point_serializer;

};


class PointSetDeserializer final : public Deserializer< std::vector<Point> >
{
public:
    PointSetDeserializer();
    PointSetDeserializer(std::shared_ptr< Deserializer<Point> > point_deserializer);
    ~PointSetDeserializer() override;

    std::vector<Point> deserialize(std::istream& stream) override;

private:
    std::shared_ptr< Deserializer<Point> > _point_deserializer;

};



}
