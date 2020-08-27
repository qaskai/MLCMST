#pragma once

#include <string>

#include <lp/mlcmst_lp_solver.hpp>
#include <lp/lp_solver_factory.hpp>

namespace MLCMST::lp {

class CapacityIndexed : public MLCMST_LPSolver
{
public:
    static std::string id();

    explicit CapacityIndexed(bool exact_solution);
    explicit CapacityIndexed(LPSolverFactory mp_solver_factory);
    ~CapacityIndexed() override;

    void printVariableSolutionValue(std::ostream &out) override;

private:
    std::vector<int> _first_facility_with_capacity;

    const std::string ARC_VAR_NAME = "arc";

    std::vector<std::vector<std::vector<LinearExpr>>> _arc_vars;

    void setupLocalVariables() override;
    void createVariables() override;
    void createConstraints() override;
    void createObjective() override;
    network::MLCST createMLCMST() override;


    void createOneOutgoingConstraints();
    void createCapacityBalanceConstraints();
};

}
