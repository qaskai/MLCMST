#pragma once

#include <optional>

#include <network/mlcc_network.hpp>
#include <network/mlcmst.hpp>

namespace MLCMST {

class MLCMST_Solver
{
public:
    struct Result
    {
        std::optional<network::MLCMST> mlcmst;
        std::optional<double> lower_bound;
        double wall_time; // in milliseconds
        bool finished;
        Result(std::optional<network::MLCMST> mlcmst, std::optional<double> lower_bound, double wall_time, bool finished);
    };

    virtual ~MLCMST_Solver();

    virtual Result solve(const network::MLCCNetwork& mlcc_network) = 0;
};

}
