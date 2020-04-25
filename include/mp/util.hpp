#pragma once

#include <vector>

#include <network/mlcc_network.hpp>
#include <network/mlcmst.hpp>

#include <ortools/linear_solver/linear_solver.h>

namespace MLCMST::mp::util {

operations_research::LinearExpr createDefaultObjectiveExpression(
    const network::MLCCNetwork& mlcc_network,
    const std::vector<std::vector<std::vector<operations_research::LinearExpr>>>& arc_vars
);
network::MLCMST createMLCMST(
    const network::MLCCNetwork& mlcc_network,
    const std::vector<std::vector<std::vector<operations_research::LinearExpr>>>& arc_vars
);
std::vector<operations_research::LinearExpr> variablesToExpr(const std::vector<operations_research::MPVariable*>& vars);

}
