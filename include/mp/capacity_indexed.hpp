#pragma once

#include <string>

#include <mp/mp_mlcmst_solver.hpp>
#include <mp/mp_solver_factory.hpp>

namespace MLCMST::mp {

class CapacityIndexed : public MP_MLCMSTSolver
{
public:
    static std::string id();

    explicit CapacityIndexed(bool exact_solution);
    explicit CapacityIndexed(MPSolverFactory mp_solver_factory);
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
    network::MLCMST createMLCMST() override;


    void createOneOutgoingConstraints();
    void createCapacityBalanceConstraints();
};

}
