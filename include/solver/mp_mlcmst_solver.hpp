#pragma once

#include <memory>

#include <solver/mlcmst_solver.hpp>

#include <mp/mp_solver.hpp>

namespace MLCMST::solver {

class MP_MLCMSTSolver : public MLCMSTSolver
{
public:
    ~MP_MLCMSTSolver() override;
    MLCMSTSolver::Result solve(const network::MLCCNetwork& mlcc_network) final;

protected:
    std::unique_ptr< mp::MPSolver > _mp_solver;

    explicit MP_MLCMSTSolver(bool exact_solution);
    explicit MP_MLCMSTSolver(std::unique_ptr< mp::MPSolver > mp_solver);

    virtual void setupLocalVariables(const network::MLCCNetwork& mlcc_network) = 0;
    virtual void createVariables() = 0;
    virtual void createConstraints() = 0;
    virtual void createObjective() = 0;
    virtual network::MLCMST createMLCMST() = 0;
};

}
