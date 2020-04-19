#include <catch2/catch.hpp>

#include <mp/or_mip_solver.hpp>

using namespace MLCMST::mp;

/**
 * Tested integer program.
 * objective:
 * x + 10 * y
 * constraints:
 * x + 7 * y <= 17.5
 * x <= 3.5
 * x >= 0
 * y >= 1
 */
TEST_CASE( "or-tools mixed integer programming wrapper | IP correctness", "[mp][or-tools][mip]" )
{
    ORMIPSolver solver;
    const double infinity = solver.infinity();

    solver.makeIntVariable(0.0, infinity, "x");
    solver.makeIntVariable(1.0, infinity, "y");

    solver.makeConstraint(-infinity, 17.5, "c0");
    solver.setConstraintCoefficient(1, "x", "c0");
    solver.setConstraintCoefficient(7, "y", "c0");
    solver.makeConstraint(-infinity, 3.5, "c1");
    solver.setConstraintCoefficient(1, "x", "c1");

    solver.setObjectiveCoefficient(1, "x");
    solver.setObjectiveCoefficient(10, "y");

    SECTION( "minimization" ) {
        double expected_variable_value[2] = { 0, 1 };
        double expected_objective_value = 10;

        solver.setMinimization();
        solver.solve();

        REQUIRE( solver.resultStatus() == ORMIPSolver::ResultStatus::OPTIMAL );
        REQUIRE( solver.objectiveValue() == expected_objective_value );
        REQUIRE( solver.variableValue("x") == expected_variable_value[0] );
        REQUIRE( solver.variableValue("y") == expected_variable_value[1] );
    }
    SECTION( "maximization" ) {
        double expected_variable_value[2] = { 3,2 };
        double expected_objective_value = 23;

        solver.setMaximization();
        solver.solve();

        REQUIRE( solver.resultStatus() == ORMIPSolver::ResultStatus::OPTIMAL );
        REQUIRE( solver.objectiveValue() == expected_objective_value );
        REQUIRE( solver.variableValue("x") == expected_variable_value[0] );
        REQUIRE( solver.variableValue("y") == expected_variable_value[1] );
    }
}


/**
 * Tested mixed integer program.
 * objective:
 * x + 10 * y
 * constraints:
 * x + 7 * y <= 17.5
 * x <= 3.5
 * x >= 0
 * y >= 1 - integer
 */
TEST_CASE( "or-tools mixed integer programming wrapper | MIP correctness", "[mp][or-tools][mip]" )
{
    const double error_epsilon = 0.001;

    ORMIPSolver solver;
    const double infinity = solver.infinity();

    solver.makeNumVariable(0.0, infinity, "x");
    solver.makeIntVariable(1.0, infinity, "y");

    solver.makeConstraint(-infinity, 17.5, "c0");
    solver.setConstraintCoefficient(1, "x", "c0");
    solver.setConstraintCoefficient(7, "y", "c0");
    solver.makeConstraint(-infinity, 3.5, "c1");
    solver.setConstraintCoefficient(1, "x", "c1");

    solver.setObjectiveCoefficient(1, "x");
    solver.setObjectiveCoefficient(10, "y");

    SECTION( "minimization" ) {
        double expected_variable_value[2] = { 0, 1 };
        double expected_objective_value = 10;

        solver.setMinimization();
        solver.solve();

        REQUIRE( solver.resultStatus() == ORMIPSolver::ResultStatus::OPTIMAL );
        REQUIRE( solver.objectiveValue() == expected_objective_value );
        REQUIRE( solver.variableValue("x") == expected_variable_value[0] );
        REQUIRE( solver.variableValue("y") == expected_variable_value[1] );
    }
    SECTION( "maximization" ) {
        double expected_variable_value[2] = { 3.5,2 };
        double expected_objective_value = 23.5;

        solver.setMaximization();
        solver.solve();

        REQUIRE( solver.resultStatus() == ORMIPSolver::ResultStatus::OPTIMAL );
        REQUIRE( solver.objectiveValue() == expected_objective_value );
        REQUIRE( solver.variableValue("x") == Approx(expected_variable_value[0]).epsilon(error_epsilon) );
        REQUIRE( solver.variableValue("y") == expected_variable_value[1] );
    }
}

TEST_CASE( "or-tools mixed integer programming wrapper | basic functionality", "[mp][or-tools][mip]" )
{
    ORMIPSolver solver;

    SECTION( "default variable creation" ) {
        solver.makeVariable(0, 1, "x");

        REQUIRE( solver.isInteger("x") );
    }
    SECTION( "default variable array creation" ) {
        solver.makeVariableArray(2, 0, 1, "arr");

        REQUIRE( solver.isInteger("arr", 0) );
        REQUIRE( solver.isInteger("arr", 1) );
    }
}
