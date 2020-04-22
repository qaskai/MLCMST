#include <catch2/catch.hpp>

#include <mp/or_mp_solver.hpp>

#include <ortools/linear_solver/linear_solver.h>

using namespace MLCMST::mp;

struct Solver : public ORMPSolver
{
    Solver() : ORMPSolver(operations_research::MPSolver::CBC_MIXED_INTEGER_PROGRAMMING) {}
    void makeVariable(double, double, std::string) override {}
    void makeVariableArray(int, double, double, std::string) override {}
};

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

TEST_CASE( "or-tools wrapper | variable properties", "[mp][or-tools]" )
{
    Solver solver;

    SECTION( "individual variable is integer" )
    {
        solver.makeIntVariable(0, 1, "x");
        solver.makeNumVariable(0, 1, "y");

        REQUIRE( solver.isInteger("x") );
        REQUIRE( !solver.isInteger("y") );
    }
    SECTION( "array variable is integer" )
    {
        solver.makeIntVariableArray(2, 0, 1, "arr0");
        solver.makeNumVariableArray(2, 0, 1, "arr1");

        REQUIRE( solver.isInteger("arr0", 0) );
        REQUIRE( solver.isInteger("arr0", 1) );
        REQUIRE( !solver.isInteger("arr1", 0) );
        REQUIRE( !solver.isInteger("arr1", 1) );
    }
}

TEST_CASE(  "or-tools wrapper | constraint properties", "[mp][or-tools]" )
{
    Solver solver;

    solver.makeNumVariable(1.0, 5.0, "var");
    solver.setObjectiveCoefficient(1, "var");

    SECTION( "individual constraint bound modification" ) {
        solver.makeConstraint("constraint");
        solver.setConstraintCoefficient(1, "var", "constraint");
        solver.setConstraintBounds(3, 4, "constraint");
        solver.setMinimization();

        solver.solve();

        REQUIRE( solver.resultStatus() == Solver::OPTIMAL );
        REQUIRE( solver.objectiveValue() == Approx(3).margin(0.0001) );
    }
    SECTION( "array constraint bound modification" ) {
        solver.makeConstraintArray(2, "constraint");
        solver.setConstraintCoefficient(1, "var", "constraint", 0);
        solver.setConstraintCoefficient(1, "var", "constraint", 1);
        solver.setConstraintBounds(2, 5, "constraint", 0);
        solver.setConstraintBounds(3, 4, "constraint", 1);
        solver.setMaximization();

        solver.solve();

        REQUIRE( solver.resultStatus() == Solver::OPTIMAL );
        REQUIRE( solver.objectiveValue() == Approx(4).margin(0.0001) );
    }
}

TEST_CASE( "or-tools wrapper | utils", "[mp][or-tools]" )
{
    Solver solver;

    solver.makeNumVariable(1.0, 2.0, "var");
    solver.makeConstraint(1.5, 1.7, "constraint");
    solver.setConstraintCoefficient(1, "var", "constraint");
    solver.setObjectiveCoefficient(1, "var");
    solver.setMaximization();

    solver.solve();

    SECTION( "timer" ) {
        REQUIRE( solver.wallTime() < 100. );
        REQUIRE( solver.objectiveValue() == Approx(1.7).margin(0.0001) );
    }
    SECTION( "reset" ) {
        REQUIRE( solver.objectiveValue() == Approx(1.7).margin(0.0001) );

        solver.reset();

        solver.makeNumVariable(1.0, 2.0, "var2");
        solver.makeConstraint(1.7, 1.8, "constraint2");
        solver.setConstraintCoefficient(1, "var2", "constraint2");
        solver.setObjectiveCoefficient(1, "var2");
        solver.setMaximization();

        solver.solve();

        REQUIRE( solver.objectiveValue() == Approx(1.8).margin(0.0001) );
    }
}
