#pragma once

#include <vector>
#include <tuple>
#include <string>

#include <mp/mp_mlcmst_solver.hpp>

namespace MLCMST::mp {

/**
 * Multicommodity formulation
 *
 * MLCMST solver
 */
class MCF : public MP_MLCMSTSolver
{
public:
    explicit MCF(bool exact_solution=false);
    explicit MCF(MLCMST::mp::MPSolverFactory mp_solver_factory);
    ~MCF() override;

private:
    int _levels_number;
    std::vector<int> _vertex_set, _commodity_set;
    std::vector<std::tuple<int,int>> _arc_set;
    std::vector<int> _supply; // at center index it is a demand

    const std::string FLOW_VAR_NAME = "flow", ARC_VAR_NAME = "arc", ARC_EXISTENCE_VAR_NAME = "arc_existence";
    std::vector<std::vector<std::vector<LinearExpr>>> _flow_vars;
    std::vector<std::vector<std::vector<LinearExpr>>> _arc_vars;
    std::vector<std::vector<LinearExpr>> _arc_existence_vars;

    void setupLocalVariables() override;
    void createVariables() override;
    void createObjective() override;
    void createConstraints() override;
    network::MLCMST createMLCMST() override;

    void printVariableSolutionValue(std::ostream& out) override;

    void createFlowConstraints();
    void createCapacityConstraints();
    void createOneOutgoingConstraints();
    void createOneBetweenConstraints();
    void createOneEdgeTypeConstraints();
    void createFacilityUtilizationConstraints();

};

}
