#pragma once

#include <mp/ORMPSolver.hpp>

namespace MLCMST {
namespace mp {

class ORLPSolver : public ORMPSolver
{
public:
    ORLPSolver();
    ~ORLPSolver() override;

    void makeIntVariable(double lb, double ub, std::string name) override;
    void makeIntVariableArray(int size, double lb, double ub, std::string name) override;
};

}
}
