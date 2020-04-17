#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <mp/MPSolver.hpp>

#include <ortools/linear_solver/linear_solver.h>

namespace MLCMST {
namespace mp {

namespace or_ = operations_research;

class ORMPSolver : public MPSolver
{
public:
    void addIntVariable(std::string name) override;
    void addIntVariableArray(int size, std::string name) override;
    void addNumVariable(std::string name) override;
    void addNumVariableArray(int size, std::string name) override;

    void addConstraint(double lb, double ub, std::string name) override;
    void addConstraintArray(int size, double lb, double ub, std::string name) override;
    void setConstraintCoefficient(double c, std::string var_name, std::string constraint_name) override;
    void setConstraintCoefficient(double c, std::string var_name, int var_i, std::string constraint_name) override;
    void setConstraintCoefficient(double c, std::string var_name, std::string constraint_name, int cons_i) override;
    void setConstraintCoefficient(double c, std::string var_name, int var_i, std::string constraint_name, int cons_i) override;

    void setMinimization() override;
    void setMaximization() override;
    void setObjectiveCoefficient(double c, std::string var_name) override;
    void setObjectiveCoefficient(double c, std::string var_name, int var_i) override;

    void solve() override;
    ResultStatus resultStatus() override;
    double objectiveValue() override;
    double variableValue(std::string name) override;
    double variableValue(std::string name, int var_i) override;

protected:
    ORMPSolver(or_::MPSolver::OptimizationProblemType problemType);
    ~ORMPSolver() override;

private:
    or_::MPSolver solver;
    std::unordered_map<std::string, or_::MPVariable*> _variables;
    std::unordered_map<std::string, std::vector<or_::MPVariable>> _variable_arrays;
    std::unordered_map<std::string, or_::MPConstraint*> _constraints;
    std::unordered_map<std::string, std::vector<or_::MPConstraint*>> _constraint_arrays;
    or_::MPObjective* _objective;

};

}
}
