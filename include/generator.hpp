#pragma once

namespace MLCMST {

template <typename T>
class Generator
{
public:
    virtual ~Generator() {}

    virtual T generate() = 0;
};

}
