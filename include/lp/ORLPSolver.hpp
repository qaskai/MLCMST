#pragma once

#include <lp/LPSolver.hpp>

namespace MLCMST {
namespace lp {

class ORLPSolver final : public LPSolver
{
public:
    ~LPSolver() override {};
    virtual void addVariable(std::string id, double low, double high) override {};
    virtual void addVariable(std::string id, double low, double high, VariableType var_type) override {};
    virtual void addConstraint(std::unordered_map<std::string, double> coefficients, double bound) override {};
    virtual LPResult minimize(std::unordered_map<std::string, double> target_coefficients) override {};
    virtual LPResult maximize(std::unordered_map<std::string, double> target_coefficients) override {};
};

}
}
