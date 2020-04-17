#include "ortools/linear_solver/linear_solver.h"

namespace operations_research {
void simple_mip_program() {
    // Create the mip solver with the CBC backend.
    MPSolver solver("simple_mip_program",
                    MPSolver::CBC_MIXED_INTEGER_PROGRAMMING);

    const double infinity = solver.infinity();
    // x and y are integer non-negative variables.
    MPVariable* const x = solver.MakeIntVar(0.0, infinity, "x");
    MPVariable* const y = solver.MakeIntVar(0.0, infinity, "y");

    LOG(INFO) << "Number of variables = " << solver.NumVariables();

    // x + 7 * y <= 17.5.
    MPConstraint* const c0 = solver.MakeRowConstraint(-infinity, 17.5, "c0");
    c0->SetCoefficient(x, 1);
    c0->SetCoefficient(y, 7);

    // x <= 3.5.
    MPConstraint* const c1 = solver.MakeRowConstraint(-infinity, 3.5, "c1");
    c1->SetCoefficient(x, 1);
//    c1->SetCoefficient(y, 0);

    LOG(INFO) << "Number of constraints = " << solver.NumConstraints();

    // Maximize x + 10 * y.
    MPObjective* const objective = solver.MutableObjective();
    objective->SetCoefficient(x, 1);
    objective->SetCoefficient(y, 10);
    objective->SetMaximization();

    const MPSolver::ResultStatus result_status = solver.Solve();
    // Check that the problem has an optimal solution.
    if (result_status != MPSolver::OPTIMAL) {
        LOG(FATAL) << "The problem does not have an optimal solution!";
    }

    LOG(INFO) << "Solution:";
    LOG(INFO) << "Objective value = " << objective->Value();
    LOG(INFO) << "x = " << x->solution_value();
    LOG(INFO) << "y = " << y->solution_value();

    LOG(INFO) << "\nAdvanced usage:";
    LOG(INFO) << "Problem solved in " << solver.wall_time() << " milliseconds";
    LOG(INFO) << "Problem solved in " << solver.iterations() << " iterations";
    LOG(INFO) << "Problem solved in " << solver.nodes()
              << " branch-and-bound nodes";
}
}  // namespace operations_research

int main(int argc, char** argv) {
    operations_research::simple_mip_program();
    return EXIT_SUCCESS;
}