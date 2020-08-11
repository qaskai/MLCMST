#pragma once

#include <memory>
#include <ostream>

#include <ortools/linear_solver/linear_solver.h>

#include <mlcmst_solver.hpp>
#include <mp/mp_solver_factory.hpp>

namespace MLCMST::mp {

class MLCMST_MPSolver : public MLCMST_Solver
{
public:
    ~MLCMST_MPSolver() override;

    bool setThreadNum(int thread_num);
    MLCMST_Solver::Result solve(const network::MLCCNetwork& mlcc_network) final;

    virtual void printVariableSolutionValue(std::ostream& out) = 0;

protected:
    using MPSolver = operations_research::MPSolver;
    using MPVariable = operations_research::MPVariable;
    using MPConstraint = operations_research::MPConstraint;
    using LinearExpr = operations_research::LinearExpr;
    using LinearRange = operations_research::LinearRange;

    const unsigned int MAX_VAR_NAME_LEN = 1000;

    MPSolver _mp_solver;
    const network::MLCCNetwork* _mlcc_network;

    explicit MLCMST_MPSolver(bool exact_solution);
    explicit MLCMST_MPSolver(mp::MPSolverFactory mp_solver_factory);

    virtual void setupLocalVariables() = 0;
    virtual void createVariables() = 0;
    virtual void createConstraints() = 0;
    virtual void createObjective() = 0;

    virtual network::MLCST createMLCMST() = 0;
};

}
