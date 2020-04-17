#pragma once

#include <string>
#include <unordered_map>

namespace MLCMST {
namespace mp {

class MPSolver
{
public:
    const double infinity;
    struct ResultStatus
    {

    };

    MPSolver(double infinity);
    virtual ~MPSolver();

    virtual void addIntVariable(std::string name) = 0;
    virtual void addIntVariableArray(int size, std::string name) = 0;
    virtual void addNumVariable(std::string name) = 0;
    virtual void addNumVariableArray(int size, std::string name) = 0;

    virtual void addConstraint(double lb, double ub, std::string name) = 0;
    virtual void addConstraintArray(int size, double lb, double ub, std::string name) = 0;
    virtual void setConstraintCoefficient(double c, std::string var_name, std::string constraint_name) = 0;
    virtual void setConstraintCoefficient(double c, std::string var_name, int var_i, std::string constraint_name) = 0;
    virtual void setConstraintCoefficient(double c, std::string var_name, std::string constraint_name, int cons_i) = 0;
    virtual void setConstraintCoefficient(double c, std::string var_name, int var_i, std::string constraint_name, int cons_i) = 0;

    virtual void setMinimization() = 0;
    virtual void setMaximization() = 0;
    virtual void setObjectiveCoefficient(double c, std::string var_name) = 0;
    virtual void setObjectiveCoefficient(double c, std::string var_name, int var_i) = 0;

    virtual void solve() = 0;
    virtual ResultStatus resultStatus() = 0;
    virtual double objectiveValue() = 0;
    virtual double variableValue(std::string name) = 0;
    virtual double variableValue(std::string name, int var_i) = 0;
};

}
}
