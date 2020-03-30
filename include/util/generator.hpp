#pragma once

namespace MLCMST {
namespace util {

template <typename T>
class Generator
{
public:
    virtual ~Generator() {}

    virtual T generate() = 0;
};

}
}
