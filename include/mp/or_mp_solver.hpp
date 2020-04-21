#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <mp/mp_solver.hpp>

#include <ortools/linear_solver/linear_solver.h>

namespace MLCMST::mp {

namespace or_ = operations_research;

class ORMPSolver : public MPSolver
{
public:
    ~ORMPSolver() override;

    double infinity() override;

    void makeIntVariable(double lb, double ub, std::string name) override;
    void makeIntVariableArray(int size, double lb, double ub, std::string name) override;
    void makeNumVariable(double lb, double ub, std::string name) override;
    void makeNumVariableArray(int size, double lb, double ub, std::string name) override;

    bool isInteger(std::string var_name) override;
    bool isInteger(std::string var_name, int var_i) override;

    void makeConstraint(std::string name) override;
    void makeConstraint(double lb, double ub, std::string name) override;
    void makeConstraintArray(int size, std::string name) override;
    void makeConstraintArray(int size, double lb, double ub, std::string name) override;
    void setConstraintBounds(double lb, double ub, std::string name) override;
    void setConstraintBounds(double lb, double ub, std::string name, int cons_i) override;
    void setConstraintCoefficient(double c, std::string var_name, std::string constraint_name) override;
    void setConstraintCoefficient(double c, std::string var_name, int var_i, std::string constraint_name) override;
    void setConstraintCoefficient(double c, std::string var_name, std::string constraint_name, int cons_i) override;
    void setConstraintCoefficient(double c, std::string var_name, int var_i, std::string constraint_name, int cons_i) override;

    void setMinimization() override;
    void setMaximization() override;
    void setObjectiveCoefficient(double c, std::string var_name) override;
    void setObjectiveCoefficient(double c, std::string var_name, int var_i) override;

    ResultStatus resultStatus() override;
    double wallTime() override;
    double objectiveValue() override;
    double variableValue(std::string name) override;
    double variableValue(std::string name, int var_i) override;

    void solve() override;

protected:
    ORMPSolver(or_::MPSolver::OptimizationProblemType problemType);

private:
    or_::MPSolver _solver;
    std::unordered_map<std::string, or_::MPVariable*> _variables;
    std::unordered_map<std::string, std::vector<or_::MPVariable*>> _variable_arrays;
    std::unordered_map<std::string, or_::MPConstraint*> _constraints;
    std::unordered_map<std::string, std::vector<or_::MPConstraint*>> _constraint_arrays;
    or_::MPObjective* _objective;
    or_::MPSolver::ResultStatus _result_status;
    double _wall_time;

    std::string constraintArrayMemberName(int i, std::string arr_name);
};

}
