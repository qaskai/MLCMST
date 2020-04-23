#pragma once

#include <memory>
#include <string>

#include <ortools/linear_solver/linear_solver.h>

namespace MLCMST::mp {

namespace or_ = operations_research;

class MPSolverFactory
{
public:
    MPSolverFactory(bool mip);

    std::unique_ptr< or_::MPSolver > create();
    std::unique_ptr< or_::MPSolver > create(or_::MPSolver::OptimizationProblemType problem_type);
    std::unique_ptr< or_::MPSolver > create(std::string name, or_::MPSolver::OptimizationProblemType problem_type);

private:
    bool _mip;
};

}
