#include <mp/mcf.hpp>

#include <util/util.hpp>
#include <mp/util.hpp>

namespace MLCMST::mp {


MCF::MCF(bool exact_solution) : MP_MLCMSTSolver(exact_solution)
{
}

MCF::MCF(MLCMST::mp::MPSolverFactory mp_solver_factory) : MP_MLCMSTSolver(mp_solver_factory)
{
}

MCF::~MCF() = default;

void MCF::setupLocalVariables()
{
    _vertex_count = _mlcc_network->vertexCount();
    _network_size = _vertex_count * _vertex_count;
    _levels_number = _mlcc_network->levelsNumber();
    _commodity_number = _mlcc_network->vertexCount();
    _supply = std::vector<int>(_vertex_count);
    for (int i=0; i<_vertex_count; i++) {
        _supply[i] = _mlcc_network->demand(i);
    }
    _supply[_mlcc_network->center()] = 0;
    _supply[_mlcc_network->center()] = std::accumulate(_supply.begin(), _supply.end(), 0);
}

void MCF::createVariables()
{
    using MLCMST::util::break_up;
    {
        std::vector<MPVariable*> arc_existence_var;
        _mp_solver->MakeVarArray(_network_size, 0, 1, _mp_solver->IsMIP(), "arc_existence", &arc_existence_var);
        std::vector<LinearExpr> arc_existence_expr = util::variablesToExpr(arc_existence_var);
        _arc_existence_vars = break_up(_vertex_count, arc_existence_expr);
    }

    {
        std::vector<MPVariable*> arc_var;
        _mp_solver->MakeVarArray(_network_size, 0, 1, _mp_solver->IsMIP(), "arc", &arc_var);
        std::vector<LinearExpr> arc_expr = util::variablesToExpr(arc_var);
        _arc_vars = break_up(_vertex_count, break_up(_levels_number, arc_expr));
    }

    {
        std::vector<MPVariable*> flow_var;
        _mp_solver->MakeVarArray(_network_size, 0, 1, _mp_solver->IsMIP(), "flow", &flow_var);
        std::vector<LinearExpr> flow_expr = util::variablesToExpr(flow_var);
        _flow_vars = break_up(_vertex_count, break_up(_vertex_count, flow_expr));
    }
}

void MCF::createObjective()
{
    LinearExpr expr = util::createDefaultObjectiveExpression(*_mlcc_network, _arc_vars);
    auto objective = _mp_solver->MutableObjective();
    objective->MinimizeLinearExpr(expr);
}

void MCF::createConstraints()
{
}

network::MLCMST MCF::createMLCMST()
{
    return util::createMLCMST(*_mlcc_network, _arc_vars);
}


}
