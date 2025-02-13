#pragma once

#include <lp/scf.hpp>

namespace MLCMST::lp {

/**
 * Enhanced Single Commodity Formulation
 *
 * MLCMST solver
 */
class ESCF : public SCF
{
public:
    static std::string id();

    explicit ESCF(bool exact_solution=false);
    explicit ESCF(MLCMST::lp::LPSolverFactory mp_solver_factory);
    ~ESCF() override;

private:
    void createConstraints() override;
    void createEnhancedCapacityConstraints();
};

}
