#include <mp/scf.hpp>

#include <numeric>

#include <util/util.hpp>

namespace MLCMST::mp {

SCF::SCF(bool exact_solution) : MP_MLCMSTSolver(exact_solution)
{
}

SCF::SCF(MLCMST::mp::MPSolverFactory mp_solver_factory) : MP_MLCMSTSolver(mp_solver_factory)
{
}

SCF::~SCF() = default;

void SCF::setupLocalVariables(const network::MLCCNetwork &mlcc_network)
{
    _mlcc_network = &mlcc_network;
    _vertex_count = mlcc_network.vertexCount();
    _network_size = _vertex_count * _vertex_count;
    _levels_number = mlcc_network.levelsNumber();
    _supply = std::vector<int>(_vertex_count);
    for (int i=0; i<_vertex_count; i++) {
        _supply[i] = mlcc_network.demand(i);
    }
    _supply[mlcc_network.center()] = 0;
    _supply[mlcc_network.center()] = std::accumulate(_supply.begin(), _supply.end(), 0);
}

void SCF::createVariables()
{
    const double infinity = _mp_solver->infinity();

    {
        std::vector<MPVariable*> arc_vars;
        _mp_solver->MakeVarArray(_levels_number*_network_size, 0, 1, _mp_solver->IsMIP(), "arcs", &arc_vars);
        std::vector<LinearExpr> arc_var_expr;
        for (MPVariable* var : arc_vars) {
            arc_var_expr.emplace_back(var);
        }
        _arc_vars = util::break_up(_vertex_count, util::break_up(_levels_number, arc_var_expr));
    }

    {
        std::vector<MPVariable*> flow_vars;
        _mp_solver->MakeNumVarArray(_network_size, 0, infinity, "flow", &flow_vars);
        std::vector<LinearExpr> flow_var_expr;
        for (MPVariable* var : flow_vars) {
            flow_var_expr.emplace_back(var);
        }
        _flow_vars = util::break_up(_vertex_count, flow_var_expr);
    }


}

void SCF::createObjective()
{
    LinearExpr expr;
    for (int i=0; i < _vertex_count; i++) {
        for (int j=0; j < _vertex_count; j++) {
            if (i == j)
                continue;
            for (int l=0; l < _levels_number; l++) {
                double cost = _mlcc_network->network(l).edgeCost(i, j);
                expr += cost * _arc_vars[i][j][l];
            }
        }
    }
    auto objective = _mp_solver->MutableObjective();
    objective->MinimizeLinearExpr(expr);
}

void SCF::createConstraints()
{
    createDemandConstraints();
    createCapacityConstraints();
    createOneOutgoingConstraints();
    createOneBetweenConstraints();
}

void SCF::createDemandConstraints()
{
    for (int i=0; i < _vertex_count; i++) {
        LinearExpr expr;
        for (int j=0; j < _vertex_count; j++) {
            if (i == j) {
                continue;
            }
            expr += _flow_vars[j][i] - _flow_vars[i][j];
        }
        double W = i == _mlcc_network->center() ? _supply[i] : -_supply[i];
        _mp_solver->MakeRowConstraint(expr == W);
    }
}

void SCF::createCapacityConstraints()
{
    for (int i=0; i < _vertex_count; i++) {
        for (int j=0; j < _vertex_count; j++) {
            if (i == j)
                continue;
            LinearExpr lhs = _flow_vars[i][j];
            LinearExpr rhs;
            for (int l=0; l < _levels_number; l++) {
                rhs += _mlcc_network->edgeCapacity(l) * _arc_vars[i][j][l];
            }
            _mp_solver->MakeRowConstraint(lhs <= rhs);
        }
    }
}

void SCF::createOneOutgoingConstraints()
{
    for (int i=0; i < _vertex_count; i++) {
        if (i == _mlcc_network->center())
            continue;
        LinearExpr expr;
        for (int j=0; j < _vertex_count; j++) {
            if (i == j)
                continue;

            for (int l=0; l < _levels_number; l++) {
                expr += _arc_vars[i][j][l];
            }
        }
        _mp_solver->MakeRowConstraint(expr == 1.0);
    }
}

void SCF::createOneBetweenConstraints()
{
    for (int i=0; i < _vertex_count; i++) {
        for (int j = i+1; j < _vertex_count; j++) {
            if (i == _mlcc_network->center() || j == _mlcc_network->center())
                continue;

            LinearExpr expr;
            for (int l=0; l < _levels_number; l++) {
                expr += _arc_vars[i][j][l] + _arc_vars[j][i][l];
            }
            _mp_solver->MakeRowConstraint(expr <= 1.0);
        }
    }
}

network::MLCMST SCF::createMLCMST()
{
    std::vector<int> parents(_vertex_count), edge_level(_vertex_count);
    for (int i=0; i < _vertex_count; i++) {
        for (int l=0; l < _levels_number; l++) {
            for (int j=0; j < _vertex_count; j++) {
                auto var = _arc_vars[i][j][l].terms().begin()->first;
                if (var->solution_value() > 0.99) {
                    parents[i] = j;
                    edge_level[i] = l;
                    goto found_parent;
                }
            }
        }
        found_parent:;
    }
   return network::MLCMST(*_mlcc_network, parents, edge_level);
}

}
