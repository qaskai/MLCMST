#pragma once

#include <mp/or_mp_solver.hpp>

namespace MLCMST::mp {

class ORMIPSolver : public ORMPSolver
{
public:
    ORMIPSolver();
    ~ORMIPSolver() override;

    void makeVariable(double lb, double ub, std::string name) override;
    void makeVariableArray(int size, double lb, double ub, std::string name) override;
};

}
