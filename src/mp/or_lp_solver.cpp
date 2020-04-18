#include <mp/or_lp_solver.hpp>

#include <stdexcept>

#include <ortools/linear_solver/linear_solver.h>

namespace MLCMST {
namespace mp {


ORLPSolver::ORLPSolver() : ORMPSolver(operations_research::MPSolver::GLOP_LINEAR_PROGRAMMING)
{

}

ORLPSolver::~ORLPSolver() = default;

void ORLPSolver::makeIntVariable(double lb, double ub, std::string name)
{
    throw std::logic_error("Integer variables are not allowed in linear program");
}

void ORLPSolver::makeIntVariableArray(int size, double lb, double ub, std::string name)
{
    throw std::logic_error("Integer variables are not allowed in linear program");
}

}
}
