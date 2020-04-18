#include <mp/ORMIPSolver.hpp>

#include <ortools/linear_solver/linear_solver.h>

namespace MLCMST {
namespace mp {

ORMIPSolver::ORMIPSolver() : ORMPSolver(operations_research::MPSolver::CBC_MIXED_INTEGER_PROGRAMMING)
{

}

ORMIPSolver::~ORMIPSolver() = default;

}
}
