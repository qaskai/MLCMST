#pragma once

#include <mp/scf.hpp>

namespace MLCMST::mp {

/**
 * Enhanced Single Commodity Formulation
 *
 * MLCMST solver
 */
class ESCF : public SCF
{
public:
    explicit ESCF(bool exact_solution=false);
    explicit ESCF(MLCMST::mp::MPSolverFactory mp_solver_factory);
    ~ESCF() override;

private:
    void createConstraints() override;
    void createEnhancedCapacityConstraints();
};

}
