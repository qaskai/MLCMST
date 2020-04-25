#pragma once

#include <string>
#include <memory>
#include <functional>
#include <vector>

#include <mp/mp_mlcmst_solver.hpp>
#include <mp/mp_solver_factory.hpp>

namespace MLCMST::mp {

/**
 * Single Commodity Formulation
 *
 * MLCMST solver
 */
class SCF : public MP_MLCMSTSolver
{
public:
    explicit SCF(bool exact_solution=false);
    explicit SCF(MLCMST::mp::MPSolverFactory mp_solver_factory);
    ~SCF() override;

protected:
    int _levels_number;
    std::vector<int> _vertex_set;
    std::vector<std::tuple<int,int>> _arc_set;
    std::vector<int> _supply; // at center index it is a demand

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

    network::MLCMST createMLCMST() override;

};

}
