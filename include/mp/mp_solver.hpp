#pragma once

#include <string>
#include <unordered_map>

namespace MLCMST::mp {

class MPSolver
{
public:
    enum ResultStatus
    {
        /// optimal.
        OPTIMAL,
        /// feasible, or stopped by limit.
        FEASIBLE,
        /// proven infeasible.
        INFEASIBLE,
        /// proven unbounded.
        UNBOUNDED,
        /// abnormal, i.e., error of some kind.
        ABNORMAL,
        /// the model is trivially invalid (NaN coefficients, etc).
        MODEL_INVALID,
        /// not been solved yet.
        NOT_SOLVED = 6
    };

    virtual ~MPSolver();

    virtual double infinity() = 0;

    virtual void makeVariable(double lb, double ub, std::string name) = 0;
    virtual void makeVariableArray(int size, double lb, double ub, std::string name) = 0;
    virtual void makeIntVariable(double lb, double ub, std::string name) = 0;
    virtual void makeIntVariableArray(int size, double lb, double ub, std::string name) = 0;
    virtual void makeNumVariable(double lb, double ub, std::string name) = 0;
    virtual void makeNumVariableArray(int size, double lb, double ub, std::string name) = 0;

    virtual bool isInteger(std::string var_name) = 0;
    virtual bool isInteger(std::string var_name, int var_i) = 0;

    virtual void makeConstraint(std::string name) = 0;
    virtual void makeConstraint(double lb, double ub, std::string name) = 0;
    virtual void makeConstraintArray(int size, std::string name) = 0;
    virtual void makeConstraintArray(int size, double lb, double ub, std::string name) = 0;
    virtual void setConstraintBounds(double lb, double ub, std::string name) = 0;
    virtual void setConstraintBounds(double lb, double ub, std::string name, int cons_i) = 0;
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
