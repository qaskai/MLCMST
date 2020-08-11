#include <mp/capacity_indexed.hpp>

#include <stdexcept>

#include <mp/util.hpp>

#include <util/util.hpp>

namespace MLCMST::mp {

std::string CapacityIndexed::id()
{
    static const std::string id = "MP_capacity_indexed";
    return id;
}

CapacityIndexed::CapacityIndexed(bool exact_solution) : MLCMST_MPSolver(exact_solution)
{
}

CapacityIndexed::CapacityIndexed(MPSolverFactory mp_solver_factory) : MLCMST_MPSolver(mp_solver_factory)
{
}

CapacityIndexed::~CapacityIndexed() = default;

void CapacityIndexed::setupLocalVariables()
{
    _first_facility_with_capacity.resize(_mlcc_network->maxEdgeCapacity() + 1);
    for (int i=0, l=0; i <= _mlcc_network->maxEdgeCapacity(); i++) {
        while (_mlcc_network->edgeCapacity(l) < i) {
            l++;
        }
        _first_facility_with_capacity[i] = l;
    }
}

void CapacityIndexed::createVariables()
{
    const int max_capacity = _mlcc_network->maxEdgeCapacity();
    const int vertex_count = _mlcc_network->vertexCount();
    const int network_size = vertex_count*vertex_count;

    // arc type
    {
        std::vector<MPVariable*> arc_vars;
        _mp_solver.MakeVarArray((max_capacity+1)*network_size, 0, 1, _mp_solver.IsMIP(), ARC_VAR_NAME, &arc_vars);
        std::vector<LinearExpr> arc_var_expr = util::variablesToExpr(arc_vars);
        _arc_vars = MLCMST::util::break_up(vertex_count, MLCMST::util::break_up(max_capacity+1, arc_var_expr));
        for (int i : _mlcc_network->vertexSet()) {
            int idx = (i*vertex_count + i) * (max_capacity+1);
            for (int p=1; p<=max_capacity; p++) {
                arc_vars[idx + p]->SetBounds(0, 0);
            }
        }
    }
}

void CapacityIndexed::createConstraints()
{
    createOneOutgoingConstraints();
    createCapacityBalanceConstraints();
}

void CapacityIndexed::createOneOutgoingConstraints()
{
    for (int i : _mlcc_network->regularVertexSet()) {
        LinearExpr expr;
        for (int j : _mlcc_network->vertexSet()) {
            if (i == j) continue;
            for (int p=1; p <= _mlcc_network->maxEdgeCapacity(); p++) {
                expr += _arc_vars[i][j][p];
            }
        }
        _mp_solver.MakeRowConstraint(expr == 1.0);
    }
}

void CapacityIndexed::createCapacityBalanceConstraints()
{
    for (int i : _mlcc_network->regularVertexSet()) {
        LinearExpr expr;
        // outgoing edges
        for (int j : _mlcc_network->vertexSet()) {
            if (i == j) continue;
            for (int p=1; p <= _mlcc_network->maxEdgeCapacity(); p++) {
                expr -= p * _arc_vars[i][j][p];
            }
        }
        // ingoing edges
        for (int j : _mlcc_network->regularVertexSet()) {
            if (i == j) continue;
            for (int p=1; p <= _mlcc_network->maxEdgeCapacity(); p++) {
                expr += p * _arc_vars[j][i][p];
            }
        }
        _mp_solver.MakeRowConstraint(expr == -_mlcc_network->demand(i));
    }
}

void CapacityIndexed::createObjective()
{
    LinearExpr expr;
    for (int i : _mlcc_network->regularVertexSet()) {
        for (int j : _mlcc_network->vertexSet()) {
            for (int p=1; p <= _mlcc_network->maxEdgeCapacity(); p++) {
                double cost = _mlcc_network->edgeCost(i, j, _first_facility_with_capacity[p]);
                expr += cost * _arc_vars[i][j][p];
            }
        }
    }

    auto objective = _mp_solver.MutableObjective();
    objective->MinimizeLinearExpr(expr);
}

network::MLCST CapacityIndexed::createMLCMST()
{
    network::MLCST mlcmst(_mlcc_network->vertexCount(), _mlcc_network->center());
    for (int i : _mlcc_network->regularVertexSet()) {
        for (int j : _mlcc_network->vertexSet()) {
            for (int p=1; p <= _mlcc_network->maxEdgeCapacity(); p++) {
                auto var = _arc_vars[i][j][p].terms().begin()->first;
                if (std::abs(var->solution_value() - 1.0) < 1e-9) {
                    mlcmst.parent(i) = j;
                    mlcmst.edgeLevel(i) = _first_facility_with_capacity[p];
                    goto found_parent;
                }
            }
        }
        found_parent: ;
    }
    return mlcmst;
}

void CapacityIndexed::printVariableSolutionValue(std::ostream &out)
{
    throw std::logic_error("not implemented");
}


}
