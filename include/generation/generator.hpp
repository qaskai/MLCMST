#pragma once

namespace MLCMST {
namespace generation {

template <typename T>
class Generator
{
public:
    virtual ~Generator() {}

    virtual T generate() = 0;
};

}
}
