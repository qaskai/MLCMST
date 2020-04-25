#include <mp/mp_solver_factory.hpp>

namespace MLCMST::mp {

MPSolverFactory::MPSolverFactory(bool mip) : _mip(mip)
{
}


or_::MPSolver MPSolverFactory::create() {
    return create(_mip);
}

or_::MPSolver MPSolverFactory::create(bool mip)
{
    if (mip) {
        return create(or_::MPSolver::CBC_MIXED_INTEGER_PROGRAMMING);
    } else {
        return create(or_::MPSolver::GLOP_LINEAR_PROGRAMMING);
    }
}

or_::MPSolver MPSolverFactory::create(or_::MPSolver::OptimizationProblemType problem_type)
{
    return create("solver", problem_type);
}

or_::MPSolver MPSolverFactory::create(std::string name, or_::MPSolver::OptimizationProblemType problem_type)
{
    return or_::MPSolver(name, problem_type);
}

}
