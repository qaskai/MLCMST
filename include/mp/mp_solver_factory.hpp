#pragma once

#include <string>

#include <ortools/linear_solver/linear_solver.h>

namespace MLCMST::mp {

namespace or_ = operations_research;

class MPSolverFactory
{
public:
    explicit MPSolverFactory(bool mip);

    or_::MPSolver create();
    or_::MPSolver create(bool mip);
    or_::MPSolver create(or_::MPSolver::OptimizationProblemType problem_type);
    or_::MPSolver create(std::string name, or_::MPSolver::OptimizationProblemType problem_type);

private:
    bool _mip;
};

}
