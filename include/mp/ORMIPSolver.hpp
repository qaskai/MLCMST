#pragma once

#include <mp/ORMPSolver.hpp>

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
