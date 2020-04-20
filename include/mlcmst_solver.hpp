#pragma once

#include <network/mlcc_network.hpp>
#include <network/mlcmst.hpp>

namespace MLCMST {

class MLCMSTSolver
{
public:
    struct Result
    {
        network::MLCMST mlcmst;
    };

    virtual ~MLCMSTSolver();

    virtual Result solve(const network::MLCCNetwork& mlcc_network) = 0;
};

}
