#pragma once

#include <network/mlcmst.hpp>
#include <network/mlcc_network.hpp>

namespace MLCMST {

class MLCMST_Improver
{
public:
    virtual ~MLCMST_Improver();

    virtual network::MLCMST improve(network::MLCMST mlcmst, const network::MLCCNetwork& mlcc_network) = 0;
};

}
