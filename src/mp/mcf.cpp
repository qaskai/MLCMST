#include <mp/mcf.hpp>

#include <cstdlib>
#include <numeric>

#include <util/util.hpp>
#include <mp/util.hpp>

namespace MLCMST::mp {

std::string MCF::ID = "MP_MCF";

MCF::MCF(bool exact_solution) : MP_MLCMSTSolver(exact_solution)
{
}

MCF::MCF(MLCMST::mp::MPSolverFactory mp_solver_factory) : MP_MLCMSTSolver(mp_solver_factory)
{
}

MCF::~MCF() = default;

void MCF::setupLocalVariables()
{
    _levels_number = _mlcc_network->levelsNumber();
    _supply = std::vector<int>(_mlcc_network->vertexCount());
    for (int i=0; i<_mlcc_network->vertexCount(); i++) {
        _supply[i] = _mlcc_network->demand(i);
    }
    _supply[_mlcc_network->center()] = 0;
    _supply[_mlcc_network->center()] = std::accumulate(_supply.begin(), _supply.end(), 0);

    _vertex_set = MLCMST::util::firstN(_mlcc_network->vertexCount());
    _arc_set = util::createArcSet(_mlcc_network->vertexCount());
    _commodity_set = _vertex_set;
    _commodity_set.erase(std::find(_commodity_set.begin(), _commodity_set.end(), _mlcc_network->center()));
}

void MCF::createVariables()
{
    using MLCMST::util::break_up;

    const int vertex_count = _mlcc_network->vertexCount();
    const int network_size = vertex_count*vertex_count;

    // arc existence
    {
        std::vector<MPVariable*> arc_existence_var;
        _mp_solver.MakeVarArray(network_size, 0, 1, _mp_solver.IsMIP(), ARC_EXISTENCE_VAR_NAME, &arc_existence_var);
        std::vector<LinearExpr> arc_existence_expr = util::variablesToExpr(arc_existence_var);
        _arc_existence_vars = break_up(vertex_count, arc_existence_expr);
        // zero loops
        for (int i : _vertex_set) {
            int idx = i*_mlcc_network->vertexCount() + i;
            arc_existence_var[idx]->SetBounds(0, 0);
        }
    }

    // arc type
    {
        std::vector<MPVariable*> arc_var;
        _mp_solver.MakeVarArray(_levels_number*network_size, 0, 1, _mp_solver.IsMIP(), ARC_VAR_NAME, &arc_var);
        std::vector<LinearExpr> arc_expr = util::variablesToExpr(arc_var);
        _arc_vars = break_up(vertex_count, break_up(_levels_number, arc_expr));
        // zero loops
        for (int i : _vertex_set) {
            int idx = (i*vertex_count + i) * _levels_number;
            for (int l=0; l<_levels_number; l++) {
                arc_var[idx + l]->SetBounds(0, 0);
            }
        }
    }

    // commodity flow
    {
        std::vector<MPVariable*> flow_var;
        _mp_solver.MakeNumVarArray(vertex_count*network_size, 0, _mp_solver.infinity(), FLOW_VAR_NAME, &flow_var);
        std::vector<LinearExpr> flow_expr = util::variablesToExpr(flow_var);
        _flow_vars = break_up(vertex_count, break_up(vertex_count, flow_expr));
        // no flow on loops
        for (int i : _vertex_set) {
            int idx = (i*vertex_count + i) * vertex_count;
            for (int k : _commodity_set) {
                flow_var[idx+k]->SetBounds(0,0);
            }
        }
        // no flow from center
        for (int i : _vertex_set) {
            int idx = (_mlcc_network->center()*vertex_count + i) * vertex_count;
            for (int k : _commodity_set) {
                flow_var[idx+k]->SetBounds(0, 0);
            }
        }
        // no flow of nonexistent center commodity
        for (int i : _vertex_set) {
            for (int j : _vertex_set) {
                int idx = (i*vertex_count + j) * vertex_count + _mlcc_network->center();
                flow_var[idx]->SetBounds(0,0);
            }
        }
    }
}

void MCF::createObjective()
{
    LinearExpr expr = util::createDefaultObjectiveExpression(*_mlcc_network, _arc_vars);
    auto objective = _mp_solver.MutableObjective();
    objective->MinimizeLinearExpr(expr);
}

void MCF::createConstraints()
{
    createFlowConstraints();
    createCapacityConstraints();
    createOneOutgoingConstraints();
    createOneBetweenConstraints();
    createOneEdgeTypeConstraints();
    createFacilityUtilizationConstraints();
}

network::MLCMST MCF::createMLCMST()
{
    return util::createMLCMST(*_mlcc_network, _arc_vars);
}

void MCF::createFlowConstraints()
{
    // commodity flow is preserved
    for (int i : _vertex_set) {
        for (int k : _commodity_set) {
            LinearExpr expr;
            for (int j : _vertex_set) {
                if (i==j)
                    continue;
                expr += _flow_vars[j][i][k] - _flow_vars[i][j][k];
            }
            double bound = i == _mlcc_network->center() ? 1 : (i==k ? -1 : 0);
            _mp_solver.MakeRowConstraint(expr == bound);
        }
    }

    // commodity must flow through an existing edge
    for (auto [i, j] : _arc_set) {
        for (int k : _commodity_set) {
            LinearExpr lhs = _flow_vars[i][j][k];
            LinearExpr rhs = _arc_existence_vars[i][j];
            _mp_solver.MakeRowConstraint(lhs <= rhs);
        }
    }
}

void MCF::createCapacityConstraints()
{
    // edges to center
    for (int i : _vertex_set) {
        if (i == _mlcc_network->center())
            continue;
        LinearExpr lhs;
        for (int k : _commodity_set) {
            lhs += _supply[k] * _flow_vars[i][_mlcc_network->center()][k];
        }
        LinearExpr rhs;
        for (int l=0; l < _levels_number; l++) {
            rhs += _mlcc_network->edgeCapacity(l) * _arc_vars[i][_mlcc_network->center()][l];
        }
        _mp_solver.MakeRowConstraint(lhs <= rhs);
    }

    // other edges
    for (auto [i,j] : _arc_set) {
        if (j == _mlcc_network->center())
            continue;

        LinearExpr lhs;
        for (int k : _commodity_set) {
            lhs += _supply[k] * _flow_vars[i][j][k];
        }
        LinearExpr rhs;
        rhs += (_mlcc_network->edgeCapacity(_levels_number-1) - _supply[j]) * _arc_vars[i][j][_levels_number-1];
        for (int l=0; l<_levels_number-1; l++) {
            rhs += _mlcc_network->edgeCapacity(l) * _arc_vars[i][j][l];
        }
        _mp_solver.MakeRowConstraint(lhs <= rhs);
    }
}

void MCF::createOneOutgoingConstraints()
{
    for (int i : _vertex_set) {
        if (i == _mlcc_network->center())
            continue;
        LinearExpr expr;
        for (int j : _vertex_set) {
            expr += _arc_existence_vars[i][j];
        }
        _mp_solver.MakeRowConstraint(expr == 1.0);
    }
}

void MCF::createOneBetweenConstraints()
{
    std::vector<std::tuple<int,int>> undirected_edges = util::createUndirectedEdgeSet(_mlcc_network->vertexCount());

    for (auto [i,j] : undirected_edges) {
//        if (i == _mlcc_network->center() || j == _mlcc_network->center())
//            continue;
        LinearExpr expr;
        expr += _arc_existence_vars[i][j] + _arc_existence_vars[j][i];
        _mp_solver.MakeRowConstraint(expr <= 1.0);
    }
}

void MCF::createOneEdgeTypeConstraints()
{
    for (auto [i,j] : _arc_set) {
        LinearExpr lhs;
        for (int l=0; l < _levels_number; l++) {
            lhs += _arc_vars[i][j][l];
        }
        LinearExpr rhs = _arc_existence_vars[i][j];
        _mp_solver.MakeRowConstraint(lhs == rhs);
    }
}

void MCF::createFacilityUtilizationConstraints()
{
    for (auto [i,j] : _arc_set) {
        LinearExpr lhs;
        for (int k : _commodity_set) {
            lhs += _supply[k] * _flow_vars[i][j][k];
        }
        LinearExpr rhs;
        rhs += _arc_vars[i][j][0];
        for (int l=1; l<_levels_number; l++) {
            rhs += (_mlcc_network->edgeCapacity(l-1) + 1) * _arc_vars[i][j][l];
        }
        _mp_solver.MakeRowConstraint(lhs >= rhs);
    }
}

void MCF::printVariableSolutionValue(std::ostream& out)
{
    char name_buff[MAX_VAR_NAME_LEN];
    // arc_existence_var
    for (int i=0; i<_mlcc_network->vertexCount(); i++) {
        for (int j=0; j<_mlcc_network->vertexCount(); j++) {
            std::sprintf(name_buff, "%s[%d][%d]", ARC_EXISTENCE_VAR_NAME.c_str(), i, j);
            const MPVariable* var = _arc_existence_vars[i][j].terms().begin()->first;
            out << name_buff << " = " << var->solution_value() << "\n";
        }
    }
    // arc_var
    for (int i=0; i<_mlcc_network->vertexCount(); i++) {
        for (int j=0; j<_mlcc_network->vertexCount(); j++) {
            for (int l=0; l<_mlcc_network->levelsNumber(); l++) {
                std::sprintf(name_buff, "%s[%d][%d][%d]", ARC_VAR_NAME.c_str(), i, j, l);
                const MPVariable* var = _arc_vars[i][j][l].terms().begin()->first;
                out << name_buff << " = " << var->solution_value() << "\n";
            }
        }
    }
    // flow_var
    for (int i=0; i<_mlcc_network->vertexCount(); i++) {
        for (int j=0; j<_mlcc_network->vertexCount(); j++) {
            for (int k=0; k<_mlcc_network->vertexCount(); k++) {
                std::sprintf(name_buff, "%s[%d][%d][%d]", FLOW_VAR_NAME.c_str(), i, j, k);
                const MPVariable* var = _flow_vars[i][j][k].terms().begin()->first;
                out << name_buff << " = " << var->solution_value() << "\n";
            }
        }
    }
}


}
