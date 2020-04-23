#pragma once

#include <solver/mp/scf.hpp>

namespace MLCMST::solver::mp {

/**
 * Enhanced Single Commodity Formulation
 *
 * MLCMST solver
 */
class ESCF : public SCF
{
public:
    explicit ESCF(bool exact_solution=false);
    explicit ESCF(std::unique_ptr< MLCMST::mp::MPSolver > mp_solver);
    ~ESCF() override;

private:
    void createConstraints() override;
    void createEnhancedCapacityConstraints();
};

}
