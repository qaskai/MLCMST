#include <mp/mp_solver_factory.hpp>

namespace MLCMST::mp {

MPSolverFactory::MPSolverFactory(bool mip) : _mip(mip)
{
}

std::unique_ptr<or_::MPSolver> MPSolverFactory::create()
{
    if (_mip) {
        return create(or_::MPSolver::CBC_MIXED_INTEGER_PROGRAMMING);
    } else {
        return create(or_::MPSolver::GLOP_LINEAR_PROGRAMMING);
    }
}

std::unique_ptr<or_::MPSolver> MPSolverFactory::create(or_::MPSolver::OptimizationProblemType problem_type)
{
    return create("solver", problem_type);
}

std::unique_ptr<or_::MPSolver> MPSolverFactory::create(
        std::string name, or_::MPSolver::OptimizationProblemType problem_type)
{
    return std::make_unique<or_::MPSolver>(name, problem_type);
}

}
