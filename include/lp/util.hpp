#pragma once

#include <vector>
#include <tuple>

#include <network/mlcst.hpp>
#include <network/mlcc_network.hpp>

#include <ortools/linear_solver/linear_solver.h>

namespace MLCMST::lp::util {

operations_research::LinearExpr createDefaultObjectiveExpression(
    const network::MLCCNetwork& mlcc_network,
    const std::vector<std::vector<std::vector<operations_research::LinearExpr>>>& arc_vars
);
network::MLCST createMLCMST(
    const network::MLCCNetwork& mlcc_network,
    const std::vector<std::vector<std::vector<operations_research::LinearExpr>>>& arc_vars
);
std::vector<operations_research::LinearExpr> variablesToExpr(const std::vector<operations_research::MPVariable*>& vars);

std::vector<std::tuple<int, int>> createArcSet(const network::MLCCNetwork& mlcc_network);
std::vector<std::tuple<int, int>> createUndirectedEdgeSet(const network::MLCCNetwork& mlcc_network);

}
