#pragma once

#include <network/mlcc_network.hpp>

namespace MLCMST::tests {

class Util
{
public:
    static network::MLCCNetwork generateNetwork (int size, bool unit_demand, long seed);
};

}