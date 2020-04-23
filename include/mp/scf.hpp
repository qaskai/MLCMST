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

    const network::MLCCNetwork* _mlcc_network;
    int _vertex_count, _network_size, _levels_number;
    std::vector<std::vector<std::vector<LinearExpr>>> _arc_vars;
    std::vector<std::vector<LinearExpr>> _flow_vars;
    std::vector<int> _supply; // at center index it is a demand

    void createConstraints() override;

    void createDemandConstraints();
    void createCapacityConstraints();
    void createOneOutgoingConstraints();
    void createOneBetweenConstraints();

private:
    void setupLocalVariables(const network::MLCCNetwork& mlcc_network) override;
    void createVariables() override;
    void createObjective() override;

    network::MLCMST createMLCMST() override;

};

}
