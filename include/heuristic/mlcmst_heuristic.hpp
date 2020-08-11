#pragma once

#include <mlcmst_solver.hpp>

namespace MLCMST::heuristic {

class MLCMST_Heuristic : public MLCMST_Solver
{
public:
    ~MLCMST_Heuristic() override;

    Result solve(const network::MLCCNetwork &mlcc_network) final;

    virtual network::MLCST run(const network::MLCCNetwork &mlcc_network) = 0;
};

}
