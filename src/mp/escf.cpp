#include <mp/escf.hpp>

namespace MLCMST::mp {

std::string ESCF::id()
{
    static std::string id = "MP_ESCF";
    return id;
}

ESCF::ESCF(bool exact_solution) : SCF(exact_solution)
{

}

ESCF::ESCF(MLCMST::mp::MPSolverFactory mp_solver_factory) : SCF(mp_solver_factory)
{

}

ESCF::~ESCF() = default;

void ESCF::createConstraints()
{
    createDemandConstraints();
    createEnhancedCapacityConstraints();
    createOneOutgoingConstraints();
    createOneBetweenConstraints();
}

void ESCF::createEnhancedCapacityConstraints()
{
    for (auto [i,j] : _arc_set) {
        LinearExpr lhs = _flow_vars[i][j];
        LinearExpr rhs;
        rhs += _arc_vars[i][j][0];
        for (int l=1; l < _levels_number; l++) {
            rhs += (_mlcc_network->edgeCapacity(l-1) + 1) * _arc_vars[i][j][l];
        }
        _mp_solver.MakeRowConstraint(lhs >= rhs);
    }

    for (int i : _vertex_set) {
        if (i == _mlcc_network->center())
            continue;

        LinearExpr lhs = _flow_vars[i][_mlcc_network->center()];
        LinearExpr rhs;
        for (int l=0; l < _levels_number; l++) {
            rhs += _mlcc_network->edgeCapacity(l) * _arc_vars[i][_mlcc_network->center()][l];
        }
        _mp_solver.MakeRowConstraint(lhs <= rhs);
    }

    for (auto [i,j] : _arc_set) {
        if (j == _mlcc_network->center())
            continue;

        LinearExpr lhs = _flow_vars[i][j];
        LinearExpr rhs =
                (_mlcc_network->edgeCapacity(_levels_number-1) - _supply[j]) * _arc_vars[i][j][_levels_number-1];
        for (int l=0; l < _levels_number-1; l++) {
            rhs += _mlcc_network->edgeCapacity(l) * _arc_vars[i][j][l];
        }
        _mp_solver.MakeRowConstraint(lhs <= rhs);
    }
}

}
