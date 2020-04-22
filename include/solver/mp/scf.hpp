#pragma once

#include <string>
#include <memory>
#include <functional>
#include <vector>

#include <solver/mp_mlcmst_solver.hpp>
#include <mp/mp_solver.hpp>

namespace MLCMST::solver::mp {

/**
 * Single Commodity Formulation
 *
 * MLCMST solver
 */
class SCF : public MP_MLCMSTSolver
{
public:
    explicit SCF(bool exact_solution=false);
    explicit SCF(std::unique_ptr< MLCMST::mp::MPSolver > mp_solver);
    ~SCF() override;

private:
    const network::MLCCNetwork* _mlcc_network;
    int _vertex_count, _network_size;
    const std::string _arc_var_name = "arc", _flow_var_name = "flow";
    std::vector<int> _supply; // at center index it is a demand

    void setupLocalVariables(const network::MLCCNetwork& mlcc_network) override;
    void createVariables() override;
    void createObjective() override;
    void createConstraints() override;

    void createDemandConstraints();
    void createCapacityConstraints();
    void createOneOutgoingConstraints();
    void createOneBetweenConstraints();

    network::MLCMST createMLCMST() override;

};

}
