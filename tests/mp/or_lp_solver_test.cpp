#include <catch2/catch.hpp>

#include <stdexcept>

#include <mp/or_lp_solver.hpp>

using namespace MLCMST::mp;

/**
 * Tested linear program.
 * objective:
 * x + 10 * y
 * constraints:
 * x + 7 * y <= 17.5
 * x <= 3.5
 * x,y >= 0
 */

TEST_CASE( "or-tools linear programming wrapper | correctness", "[mp][or-tools][linear]" )
{
    ORLPSolver solver;
    const double infinity = solver.infinity();

    solver.makeNumVariable(0.0, infinity, "x");
    solver.makeNumVariable(0.0, infinity, "y");

    solver.makeConstraint(-infinity, 17.5, "c0");
    solver.setConstraintCoefficient(1, "x", "c0");
    solver.setConstraintCoefficient(7, "y", "c0");

    solver.makeConstraint(-infinity, 3.5, "c1");
    solver.setConstraintCoefficient(1, "x", "c1");

    solver.setObjectiveCoefficient(1, "x");
    solver.setObjectiveCoefficient(10, "y");

    SECTION( "maximization" ) {
        double expected_var_value[2] = {0, 2.5 };
        double expected_objective_value = 25;

        solver.setMaximization();
        solver.solve();

        REQUIRE( solver.resultStatus() == ORLPSolver::ResultStatus::OPTIMAL );
        REQUIRE( solver.objectiveValue() == expected_objective_value );
        REQUIRE( solver.variableValue("x") == expected_var_value[0] );
        REQUIRE( solver.variableValue("y") == expected_var_value[1] );
    }
    SECTION( "minimization" ) {
        double expected_var_value[2] = {0, 0 };
        double expected_objective_value = 0;

        solver.setMinimization();
        solver.solve();

        REQUIRE( solver.resultStatus() == ORLPSolver::ResultStatus::OPTIMAL );
        REQUIRE( solver.objectiveValue() == expected_objective_value );
        REQUIRE( solver.variableValue("x") == expected_var_value[0] );
        REQUIRE( solver.variableValue("y") == expected_var_value[1] );
    }
}

TEST_CASE( "or-tools linear programming wrapper | basic functionality", "[mp][or-tools][linear]" )
{
    ORLPSolver solver;

    SECTION( "default variable creation" ) {
        solver.makeVariable(0, 1, "x");

        REQUIRE( !solver.isInteger("x") );
    }
    SECTION( "default variable array creation" ) {
        solver.makeVariableArray(2, 0, 1, "arr");

        REQUIRE( !solver.isInteger("arr", 0) );
        REQUIRE( !solver.isInteger("arr", 1) );
    }
}

TEST_CASE( "or-tools linear programming wrapper | exceptions on integer variable creation ", "[mp][or-tools][linear]" )
{
    ORLPSolver solver;

    SECTION( "individual num variable creation throws" ) {
        REQUIRE_THROWS_AS( solver.makeIntVariable(0.0, 1.0, "x"), std::logic_error );
    }
    SECTION( "array num variable creation throws" ) {
        REQUIRE_THROWS_AS( solver.makeIntVariableArray(5, 0.0, 1.0, "var_arr"), std::logic_error );
    }
}
