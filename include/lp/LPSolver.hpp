#pragma once

#include <string>
#include <unordered_map>

namespace MLCMST {
namespace lp {

class LPSolver
{
public:
    const double infinity;
    enum VariableType
    {
        INT, REAL
    };
    struct LPResult
    {

    };

    virtual ~LPSolver() {};
    virtual void addVariable(std::string id, double low, double high) = 0;
    virtual void addVariable(std::string id, double low, double high, VariableType var_type) = 0;
    virtual void addConstraint(std::unordered_map<std::string, double> coefficients, double bound) = 0;
    virtual LPResult minimize(std::unordered_map<std::string, double> target_coefficients) = 0;
    virtual LPResult maximize(std::unordered_map<std::string, double> target_coefficients) = 0;
};

}
}
