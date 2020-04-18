#include <catch2/catch.hpp>

#include <mp/or_mp_solver.hpp>

#include <ortools/linear_solver/linear_solver.h>

using namespace MLCMST::mp;


/**
 * Tested mixed integer program.
 * objective:
 * x + 10 * y
 * constraints:
 * x + 7 * y <= 17.5
 * x <= 17.5
 * x >= 0 - natural number
 * y >= 0
 *
 */
TEST_CASE( "or-tools wrapper | variable and constraint creation, variable access", "[mp][or-tools]" )
{

    struct Solver : public ORMPSolver
    {
        Solver() : ORMPSolver(operations_research::MPSolver::CBC_MIXED_INTEGER_PROGRAMMING) {}
    };

    const double error_epsilon = 0.001;
    double expected_var_value[2] = { 0, 2.5 };
    double expected_objective_value = 25;

    Solver solver;
    const double infinity = solver.infinity();

    SECTION( "individual variables" ) {
        solver.makeIntVariable(0.0, infinity, "x");
        solver.makeNumVariable(0.0, infinity, "y");

        solver.setObjectiveCoefficient(1, "x");
        solver.setObjectiveCoefficient(10, "y");
        solver.setMaximization();

        SECTION( "individual constraints" ) {
            solver.makeConstraint(-infinity, 17.5, "c0");
            solver.setConstraintCoefficient(1, "x", "c0");
            solver.setConstraintCoefficient(7, "y", "c0");

            solver.makeConstraint(-infinity, 17.5, "c1");
            solver.setConstraintCoefficient(1, "x", "c1");

            solver.solve();

            REQUIRE( solver.resultStatus() == Solver::ResultStatus::OPTIMAL );
            REQUIRE( solver.objectiveValue() == expected_objective_value );
            REQUIRE( solver.variableValue("x") == expected_var_value[0] );
            REQUIRE( solver.variableValue("y") == Approx(expected_var_value[1]).epsilon(error_epsilon) );
        }
        SECTION( "array constraints" ) {
            solver.makeConstraintArray(2, -infinity, 17.5, "c");
            solver.setConstraintCoefficient(1, "x", "c", 0);
            solver.setConstraintCoefficient(7, "y", "c", 0);
            solver.setConstraintCoefficient(1, "x", "c", 1);

            solver.solve();

            REQUIRE( solver.resultStatus() == Solver::ResultStatus::OPTIMAL );
            REQUIRE( solver.objectiveValue() == expected_objective_value );
            REQUIRE( solver.variableValue("x") == expected_var_value[0] );
            REQUIRE( solver.variableValue("y") == Approx(expected_var_value[1]).epsilon(error_epsilon) );
        }
    }
    SECTION( "array variables" ) {
        solver.makeIntVariableArray(1, 0.0, infinity, "int_vars");
        solver.makeNumVariableArray(1, 0.0, infinity, "num_vars");

        solver.setObjectiveCoefficient(1, "int_vars", 0);
        solver.setObjectiveCoefficient(10, "num_vars", 0);
        solver.setMaximization();

        SECTION( "individual constraints" ) {
            solver.makeConstraint(-infinity, 17.5, "c0");
            solver.setConstraintCoefficient(1, "int_vars", 0,  "c0");
            solver.setConstraintCoefficient(7, "num_vars", 0,  "c0");

            solver.makeConstraint(-infinity, 17.5, "c1");
            solver.setConstraintCoefficient(1, "int_vars", 0,  "c1");

            solver.solve();

            REQUIRE( solver.resultStatus() == Solver::ResultStatus::OPTIMAL );
            REQUIRE( solver.objectiveValue() == expected_objective_value );
            REQUIRE( solver.variableValue("int_vars", 0) == expected_var_value[0] );
            REQUIRE( solver.variableValue("num_vars", 0) == Approx(expected_var_value[1]).epsilon(error_epsilon) );
        }
        SECTION( "array constraints" ) {
            solver.makeConstraintArray(2, -infinity, 17.5, "c");
            solver.setConstraintCoefficient(1, "int_vars", 0,  "c", 0);
            solver.setConstraintCoefficient(7, "num_vars", 0,  "c", 0);
            solver.setConstraintCoefficient(1, "int_vars", 0,  "c", 1);

            solver.solve();

            REQUIRE( solver.resultStatus() == Solver::ResultStatus::OPTIMAL );
            REQUIRE( solver.objectiveValue() == expected_objective_value );
            REQUIRE( solver.variableValue("int_vars", 0) == expected_var_value[0] );
            REQUIRE( solver.variableValue("num_vars", 0) == Approx(expected_var_value[1]).epsilon(error_epsilon) );
        }
    }
}
