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

TEST_CASE( "or-tools linear programming wrapper | basic functionality", "[mp][or-tools][linear]" )
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
        double expectedVarValue[2] = { 0, 2.5 };
        double expectedObjectiveValue = 25;

        solver.setMaximization();
        solver.solve();

        REQUIRE( solver.resultStatus() == ORLPSolver::ResultStatus::OPTIMAL );
        REQUIRE( solver.objectiveValue() == expectedObjectiveValue );
        REQUIRE( solver.variableValue("x") == expectedVarValue[0] );
        REQUIRE( solver.variableValue("y") == expectedVarValue[1] );
    }
    SECTION( "minimization" ) {
        double expectedVarValue[2] = { 0,0 };
        double expectedObjectiveValue = 0;

        solver.setMinimization();
        solver.solve();

        REQUIRE( solver.resultStatus() == ORLPSolver::ResultStatus::OPTIMAL );
        REQUIRE( solver.objectiveValue() == expectedObjectiveValue );
        REQUIRE( solver.variableValue("x") == expectedVarValue[0] );
        REQUIRE( solver.variableValue("y") == expectedVarValue[1] );
    }
}


/**
 * Tested linear program.
 * objective:
 * x + 10 * y
 * constraints:
 * x + 7 * y <= 17.5
 * x <= 17.5
 * x,y >= 0
 */

TEST_CASE( "or-tools linear programming wrapper | variable and constraint creation and access", "[mp][or-tools][linear]" )
{
    double expectedVarValue[2] = { 0, 2.5 };
    double expectedObjectiveValue = 25;

    ORLPSolver solver;
    const double infinity = solver.infinity();

    SECTION( "individual variables" ) {
        solver.makeNumVariable(0.0, infinity, "x");
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

            REQUIRE( solver.resultStatus() == ORLPSolver::ResultStatus::OPTIMAL );
            REQUIRE( solver.objectiveValue() == expectedObjectiveValue );
            REQUIRE( solver.variableValue("x") == expectedVarValue[0] );
            REQUIRE( solver.variableValue("y") == expectedVarValue[1] );
        }
        SECTION( "array constraints" ) {
            solver.makeConstraintArray(2, -infinity, 17.5, "c");
            solver.setConstraintCoefficient(1, "x", "c", 0);
            solver.setConstraintCoefficient(7, "y", "c", 0);
            solver.setConstraintCoefficient(1, "x", "c", 1);

            solver.solve();

            REQUIRE( solver.resultStatus() == ORLPSolver::ResultStatus::OPTIMAL );
            REQUIRE( solver.objectiveValue() == expectedObjectiveValue );
            REQUIRE( solver.variableValue("x") == expectedVarValue[0] );
            REQUIRE( solver.variableValue("y") == expectedVarValue[1] );
        }
    }
    SECTION( "array variables" ) {
        solver.makeNumVariableArray(2, 0.0, infinity, "vars");

        solver.setObjectiveCoefficient(1, "vars", 0);
        solver.setObjectiveCoefficient(10, "vars", 1);
        solver.setMaximization();

        SECTION( "individual constraints" ) {
            solver.makeConstraint(-infinity, 17.5, "c0");
            solver.setConstraintCoefficient(1, "vars", 0,  "c0");
            solver.setConstraintCoefficient(7, "vars", 1,  "c0");

            solver.makeConstraint(-infinity, 17.5, "c1");
            solver.setConstraintCoefficient(1, "vars", 0,  "c1");

            solver.solve();

            REQUIRE( solver.resultStatus() == ORLPSolver::ResultStatus::OPTIMAL );
            REQUIRE( solver.objectiveValue() == expectedObjectiveValue );
            REQUIRE( solver.variableValue("vars", 0) == expectedVarValue[0] );
            REQUIRE( solver.variableValue("vars", 1) == expectedVarValue[1] );
        }
        SECTION( "array constraints" ) {
            solver.makeConstraintArray(2, -infinity, 17.5, "c");
            solver.setConstraintCoefficient(1, "vars", 0,  "c", 0);
            solver.setConstraintCoefficient(7, "vars", 1,  "c", 0);
            solver.setConstraintCoefficient(1, "vars", 0,  "c", 1);

            solver.solve();

            REQUIRE( solver.resultStatus() == ORLPSolver::ResultStatus::OPTIMAL );
            REQUIRE( solver.objectiveValue() == expectedObjectiveValue );
            REQUIRE( solver.variableValue("vars", 0) == expectedVarValue[0] );
            REQUIRE( solver.variableValue("vars", 1) == expectedVarValue[1] );
        }
    }
}

TEST_CASE( "or-tools linear programming wrapper | exceptions for integer variable creation ", "[mp][or-tools][linear]" )
{
    ORLPSolver solver;

    SECTION( "individual num variable creation throws" ) {
        REQUIRE_THROWS_AS( solver.makeIntVariable(0.0, 1.0, "x"), std::logic_error );
    }
    SECTION( "array num variable creation throws" ) {
        REQUIRE_THROWS_AS( solver.makeIntVariableArray(5, 0.0, 1.0, "var_arr"), std::logic_error );
    }
}
