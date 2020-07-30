#pragma once

namespace MLCMST::json {

template <typename Class, typename T>
struct Property
{
    constexpr Property(T Class::*member, const char* name);

    using Type = T;

    T Class::*member;
    const char* name;
};

template<typename Class, typename T>
constexpr Property<Class, T>::Property(T Class::*member, const char *name) : member(member), name(name)
{}

}
