#include "ortools/linear_solver/linear_solver.h"

#include <mp/ORMPSolver.hpp>
#include <memory>

namespace MLCMST {
namespace mp {
void simple_mip_program() {
    // Create the mip solver with the CBC backend.
    std::unique_ptr< MPSolver > solver = std::make_unique<ORMPSolver>(
            operations_research::MPSolver::CBC_MIXED_INTEGER_PROGRAMMING);

    const double infinity = solver->infinity();
    // x and y are integer non-negative variables.
    solver->makeIntVariable(0.0, infinity, "x");
    solver->makeIntVariable(0.0, infinity, "y");

    // x + 7 * y <= 17.5.
    solver->makeConstraint(-infinity, 17.5, "c0");
    solver->setConstraintCoefficient(1, "x", "c0");
    solver->setConstraintCoefficient(7, "y", "c0");

    // x <= 3.5.
    solver->makeConstraint(-infinity, 3.5, "c1");
    solver->setConstraintCoefficient(1, "x", "c1");

    // Maximize x + 10 * y.
    solver->setObjectiveCoefficient(1, "x");
    solver->setObjectiveCoefficient(10, "y");
    solver->setMaximization();

    solver->solve();
    // Check that the problem has an optimal solution.
    if (solver->resultStatus() != MPSolver::OPTIMAL) {
        LOG(INFO) << "The problem does not have an optimal solution!";
        return;
    }

    LOG(INFO) << "Solution:";
    LOG(INFO) << "Objective value = " << solver->objectiveValue();
    LOG(INFO) << "x = " << solver->variableValue("x");
    LOG(INFO) << "y = " << solver->variableValue("y");
}
}
}



int main(int argc, char** argv) {
    MLCMST::mp::simple_mip_program();
    LOG(INFO) << "\n\n";
    return EXIT_SUCCESS;
}