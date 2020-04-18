#pragma once

#include <mp/or_mp_solver.hpp>

namespace MLCMST {
namespace mp {

class ORMIPSolver : public ORMPSolver
{
public:
    ORMIPSolver();
    ~ORMIPSolver() override;
};

}
}
