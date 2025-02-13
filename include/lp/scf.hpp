#pragma once

#include <string>
#include <memory>
#include <functional>
#include <vector>

#include <lp/mlcmst_lp_solver.hpp>
#include <lp/lp_solver_factory.hpp>

namespace MLCMST::lp {

/**
 * Single Commodity Formulation
 *
 * MLCMST solver
 */
class SCF : public MLCMST_LPSolver
{
public:
    static std::string id();

    explicit SCF(bool exact_solution=false);
    explicit SCF(MLCMST::lp::LPSolverFactory mp_solver_factory);
    ~SCF() override;

    void printVariableSolutionValue(std::ostream& out) override;

protected:
    int _levels_number;
    std::vector<int> _vertex_set;
    std::vector<std::tuple<int,int>> _arc_set;
    std::vector<int> _supply; // at center index it is a demand

    const std::string FLOW_VAR_NAME = "flow", ARC_VAR_NAME = "arc";
    std::vector<std::vector<std::vector<LinearExpr>>> _arc_vars;
    std::vector<std::vector<LinearExpr>> _flow_vars;

    void createConstraints() override;
    void createDemandConstraints();
    void createCapacityConstraints();
    void createOneOutgoingConstraints();
    void createOneBetweenConstraints();

private:
    void setupLocalVariables() override;
    void createVariables() override;
    void createObjective() override;

    network::MLCST createMLCMST() override;

};

}
