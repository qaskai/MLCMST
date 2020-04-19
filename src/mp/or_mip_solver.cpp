#include <mp/or_mip_solver.hpp>

#include <ortools/linear_solver/linear_solver.h>

namespace MLCMST {
namespace mp {

ORMIPSolver::ORMIPSolver() : ORMPSolver(operations_research::MPSolver::CBC_MIXED_INTEGER_PROGRAMMING)
{

}

ORMIPSolver::~ORMIPSolver() = default;

void ORMIPSolver::makeVariable(double lb, double ub, std::string name)
{
    makeIntVariable(lb, ub, name);
}

void ORMIPSolver::makeVariableArray(int size, double lb, double ub, std::string name)
{
    makeIntVariableArray(size, lb, ub, name);
}

}
}
