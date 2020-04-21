#pragma once

#include <mp/or_mp_solver.hpp>

namespace MLCMST::mp {

class ORLPSolver : public ORMPSolver
{
public:
    ORLPSolver();
    ~ORLPSolver() override;

    void makeVariable(double lb, double ub, std::string name) override;
    void makeVariableArray(int size, double lb, double ub, std::string name) override;
    void makeIntVariable(double lb, double ub, std::string name) override;
    void makeIntVariableArray(int size, double lb, double ub, std::string name) override;
};

}
