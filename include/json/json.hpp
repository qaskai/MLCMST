#pragma once

#include <rapidjson/document.h>

#include <json/property.hpp>

namespace MLCMST::json {

template <typename T>
T fromJson(const rapidjson::Value& data);


namespace internal {

template <std::size_t i, typename T>
void setDataPropertyI(T&& object, const rapidjson::Value& data);

template <std::size_t i, typename T, std::enable_if_t<(i > 0)>* = nullptr >
void setData(T&& object, const rapidjson::Value& data);
template <std::size_t i, typename T, std::enable_if_t<(i == 0)>* = nullptr >
void setData(T&& object, const rapidjson::Value& data);

template <std::size_t i, typename T>
void setDataPropertyI(T&& object, const rapidjson::Value& data)
{
    constexpr auto property = std::get<i>(std::decay_t<T>::properties);
    using Type = typename decltype(property)::Type;
    if (!data.HasMember(property.name)) {
        throw std::invalid_argument(std::string("missing ") + property.name + std::string(" property"));
    }
    object.*(property.member) = fromJson<Type>(data[property.name]);
}

template <std::size_t i, typename T, std::enable_if_t<(i > 0)>* >
void setData(T&& object, const rapidjson::Value& data)
{
    setDataPropertyI<i, T>(object, data);
    setData<i-1>(object, data);
}

template <std::size_t i, typename T, std::enable_if_t<(i == 0)>* >
void setData(T&& object, const rapidjson::Value& data)
{
    setDataPropertyI<i>(object, data);
}

}

template <typename T>
T fromJson(const rapidjson::Value& data)
{
    T object;
    internal::setData<std::tuple_size<decltype(T::properties)>::value - 1>(object, data);
    return object;
}

template <>
std::string fromJson<std::string>(const rapidjson::Value& data)
{
    return data.GetString();
}

template <>
int fromJson<int>(const rapidjson::Value& data)
{
    return data.GetInt();
}

template <>
double fromJson<double>(const rapidjson::Value& data)
{
    return data.GetDouble();
}

template <>
bool fromJson<bool>(const rapidjson::Value& data)
{
    return data.GetBool();
}

}
