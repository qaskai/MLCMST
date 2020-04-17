#include <mp/ORMPSolver.hpp>

namespace MLCMST {
namespace mp {

ORMPSolver::ORMPSolver(or_::MPSolver::OptimizationProblemType problemType)
    : _solver("wrapped_MPSolver", problemType)
{
    _objective = _solver.MutableObjective();
    _result_status = or_::MPSolver::NOT_SOLVED;
}

ORMPSolver::~ORMPSolver() = default;

double ORMPSolver::infinity() {
    return _solver.infinity();
}

void ORMPSolver::makeIntVariable(double lb, double ub, std::string name)
{
    _variables[name] = _solver.MakeIntVar(lb, ub, name);
}

void ORMPSolver::makeIntVariableArray(int size, double lb, double ub, std::string name)
{
    _variable_arrays[name].clear();
    _solver.MakeIntVarArray(size, lb, ub, name, &(_variable_arrays[name]));
}

void ORMPSolver::makeNumVariable(double lb, double ub, std::string name)
{
    _variables[name] = _solver.MakeIntVar(lb, ub, name);
}

void ORMPSolver::makeNumVariableArray(int size, double lb, double ub, std::string name)
{
    _variable_arrays[name].clear();
    _solver.MakeIntVarArray(size, lb, ub, name, &(_variable_arrays[name]));
}

void ORMPSolver::makeConstraint(double lb, double ub, std::string name)
{
    _constraints[name] = _solver.MakeRowConstraint(lb, ub, name);
}

void ORMPSolver::makeConstraintArray(int size, double lb, double ub, std::string name)
{
    std::vector<or_::MPConstraint*>& constraint_arr = _constraint_arrays[name];
    constraint_arr.clear();
    for (int i=0; i<size; i++) {
        constraint_arr.push_back(_solver.MakeRowConstraint(lb, ub, constraintArrayMemberName(i, name)));
    }
}

void ORMPSolver::setConstraintCoefficient(double c, std::string var_name, std::string constraint_name)
{
    _constraints[constraint_name]->SetCoefficient(_variables[var_name], c);
}

void ORMPSolver::setConstraintCoefficient(double c, std::string var_name, int var_i, std::string constraint_name)
{
    _constraints[constraint_name]->SetCoefficient(_variable_arrays[var_name][var_i], c);
}

void ORMPSolver::setConstraintCoefficient(double c, std::string var_name, std::string constraint_name, int cons_i)
{
    _constraint_arrays[constraint_name][cons_i]->SetCoefficient(_variables[var_name], c);
}

void ORMPSolver::setConstraintCoefficient(double c, std::string var_name, int var_i, std::string constraint_name,
                                          int cons_i)
{
    _constraint_arrays[constraint_name][cons_i]->SetCoefficient(_variable_arrays[var_name][var_i], c);
}

void ORMPSolver::setMinimization()
{
    _objective->SetMinimization();
}

void ORMPSolver::setMaximization()
{
    _objective->SetMaximization();
}

void ORMPSolver::setObjectiveCoefficient(double c, std::string var_name)
{
    _objective->SetCoefficient(_variables[var_name], c);
}

void ORMPSolver::setObjectiveCoefficient(double c, std::string var_name, int var_i)
{
    _objective->SetCoefficient(_variable_arrays[var_name][var_i], c);
}

MPSolver::ResultStatus ORMPSolver::resultStatus()
{
    auto cast_enum = std::unordered_map<or_::MPSolver::ResultStatus, MPSolver::ResultStatus> {
        std::make_pair(or_::MPSolver::ResultStatus::OPTIMAL, MPSolver::ResultStatus::OPTIMAL),
        std::make_pair(or_::MPSolver::ResultStatus::FEASIBLE, MPSolver::ResultStatus::FEASIBLE),
        std::make_pair(or_::MPSolver::ResultStatus::INFEASIBLE, MPSolver::ResultStatus::INFEASIBLE),
        std::make_pair(or_::MPSolver::ResultStatus::UNBOUNDED, MPSolver::ResultStatus::UNBOUNDED),
        std::make_pair(or_::MPSolver::ResultStatus::ABNORMAL, MPSolver::ResultStatus::ABNORMAL),
        std::make_pair(or_::MPSolver::ResultStatus::MODEL_INVALID, MPSolver::ResultStatus::MODEL_INVALID),
        std::make_pair(or_::MPSolver::ResultStatus::NOT_SOLVED, MPSolver::ResultStatus::NOT_SOLVED)
    };
    return cast_enum[_result_status];
}

double ORMPSolver::objectiveValue()
{
    return _objective->Value();
}

double ORMPSolver::variableValue(std::string name)
{
    return _variables[name]->solution_value();
}

double ORMPSolver::variableValue(std::string name, int var_i)
{
    return _variable_arrays[name][var_i]->solution_value();
}

void ORMPSolver::solve()
{
    _result_status = _solver.Solve();
}

std::string ORMPSolver::constraintArrayMemberName(int i, std::string arr_name)
{
    return arr_name + "|||" + std::to_string(i);
}


}
}
