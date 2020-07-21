#include <mp/scf.hpp>

#include <numeric>

#include <util/util.hpp>
#include <mp/util.hpp>

namespace MLCMST::mp {

std::string SCF::id()
{
    static std::string id = "MP_SCF";
    return id;
}

SCF::SCF(bool exact_solution) : MP_MLCMSTSolver(exact_solution)
{
}

SCF::SCF(MLCMST::mp::MPSolverFactory mp_solver_factory) : MP_MLCMSTSolver(mp_solver_factory)
{
}

SCF::~SCF() = default;

void SCF::setupLocalVariables()
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
}

void SCF::createVariables()
{
    const double infinity = _mp_solver.infinity();
    const int vertex_count = _mlcc_network->vertexCount();
    const int network_size = vertex_count*vertex_count;

    // arc type
    {
        std::vector<MPVariable*> arc_vars;
        _mp_solver.MakeVarArray(_levels_number*network_size, 0, 1, _mp_solver.IsMIP(), ARC_VAR_NAME, &arc_vars);
        std::vector<LinearExpr> arc_var_expr = util::variablesToExpr(arc_vars);
        _arc_vars = MLCMST::util::break_up(vertex_count, MLCMST::util::break_up(_levels_number, arc_var_expr));
        // zero loops
        for (int i : _vertex_set) {
            int idx = (i*vertex_count + i) * _levels_number;
            for (int l=0; l<_levels_number; l++) {
                arc_vars[idx + l]->SetBounds(0, 0);
            }
        }
    }

    // flow
    {
        std::vector<MPVariable*> flow_vars;
        _mp_solver.MakeNumVarArray(network_size, 0, infinity, FLOW_VAR_NAME, &flow_vars);
        std::vector<LinearExpr> flow_var_expr = util::variablesToExpr(flow_vars);
        _flow_vars = MLCMST::util::break_up(vertex_count, flow_var_expr);
        // no loop flow
        for (int i : _vertex_set) {
            int idx = i*vertex_count + i;
            flow_vars[idx]->SetBounds(0, 0);
        }
        // no flow from center
        for (int i : _vertex_set) {
            int idx = _mlcc_network->center()*vertex_count + i;
            flow_vars[idx]->SetBounds(0, 0);
        }
    }


}

void SCF::createObjective()
{
    LinearExpr expr = util::createDefaultObjectiveExpression(*_mlcc_network, _arc_vars);
    auto objective = _mp_solver.MutableObjective();
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
    for (int i : _vertex_set) {
        LinearExpr expr;
        for (int j : _vertex_set) {
            if (i == j) {
                continue;
            }
            expr += _flow_vars[j][i] - _flow_vars[i][j];
        }
        double W = i == _mlcc_network->center() ? _supply[i] : -_supply[i];
        _mp_solver.MakeRowConstraint(expr == W);
    }
}

void SCF::createCapacityConstraints()
{
    for (auto [i,j] : _arc_set) {
        LinearExpr lhs = _flow_vars[i][j];
        LinearExpr rhs;
        for (int l=0; l < _levels_number; l++) {
            rhs += _mlcc_network->edgeCapacity(l) * _arc_vars[i][j][l];
        }
        _mp_solver.MakeRowConstraint(lhs <= rhs);
    }
}

void SCF::createOneOutgoingConstraints()
{
    for (int i : _vertex_set) {
        if (i == _mlcc_network->center())
            continue;
        LinearExpr expr;
        for (int j : _vertex_set) {
            if (i == j)
                continue;

            for (int l=0; l < _levels_number; l++) {
                expr += _arc_vars[i][j][l];
            }
        }
        _mp_solver.MakeRowConstraint(expr == 1.0);
    }
}

void SCF::createOneBetweenConstraints() {
    for (auto [i,j] : util::createUndirectedEdgeSet(_mlcc_network->vertexCount())) {
        if (i == _mlcc_network->center() || j == _mlcc_network->center())
            continue;

        LinearExpr expr;
        for (int l = 0; l < _levels_number; l++) {
            expr += _arc_vars[i][j][l] + _arc_vars[j][i][l];
        }
        _mp_solver.MakeRowConstraint(expr <= 1.0);
    }
}

network::MLCMST SCF::createMLCMST()
{
    return util::createMLCMST(*_mlcc_network, _arc_vars);
}

void SCF::printVariableSolutionValue(std::ostream &out)
{
    char name_buff[MAX_VAR_NAME_LEN];
    // arc vars
    for (int i=0; i < _mlcc_network->vertexCount(); i++) {
        for (int j=0; j< _mlcc_network->vertexCount(); j++) {
            for (int l=0; l < _mlcc_network->levelsNumber(); l++) {
                std::sprintf(name_buff, "%s[%d][%d][%d]", ARC_VAR_NAME.c_str(), i, j, l);
                const MPVariable* var = _arc_vars[i][j][l].terms().begin()->first;
                out << name_buff << " = " << var->solution_value() << "\n";
            }
        }
    }

    // flow vars
    for (int i=0; i < _mlcc_network->vertexCount(); i++) {
        for (int j = 0; j < _mlcc_network->vertexCount(); j++) {
            std::sprintf(name_buff, "%s[%d][%d]", FLOW_VAR_NAME.c_str(), i, j);
            const MPVariable* var = _flow_vars[i][j].terms().begin()->first;
            out << name_buff << " = " << var->solution_value() << "\n";
        }
    }
}

}
