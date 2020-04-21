#pragma once

#include <string>
#include <memory>
#include <functional>
#include <vector>

#include <mlcmst_solver.hpp>
#include <mp/mp_solver.hpp>

namespace MLCMST {
namespace solver {
namespace mp {

/**
 * Single Commodity Formulation
 *
 * MLCMST solver
 */
class SCF : public MLCMSTSolver
{
public:
    SCF(bool exact_solution = false);

    Result solve(const network::MLCCNetwork& mlcc_network) override;

private:
    bool _exact_solution;

    MLCMST::mp::MPSolver* _mp_solver;
    const network::MLCCNetwork* _mlcc_network;
    int _vertex_count, _network_size;
    const std::string _arc_var_name = "arc", _flow_var_name = "flow";
    std::vector<int> _supply; // at center index it is a demand

    void setupLocalVariables(const network::MLCCNetwork& mlcc_network);
    std::unique_ptr< MLCMST::mp::MPSolver > createMPSolver();
    void createVariables();
    void createObjective();

    void createConstraints();
    void createDemandConstraints();
    void createCapacityConstraints();
    void createOneOutgoingConstraints();
    void createOneBetweenConstraints();

    Result createResultStructure();

};

}
}
}
