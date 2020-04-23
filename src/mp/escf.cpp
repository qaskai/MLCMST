#include <mp/escf.hpp>

namespace MLCMST::mp {


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

    for (int i=0; i<_vertex_count; i++) {
        for (int j=0; j<_vertex_count; j++) {
            if (i == j)
                continue;
            LinearExpr lhs = _flow_vars[i][j];
            LinearExpr rhs;
            rhs += _arc_vars[i][j][0];
            for (int l=1; l < _levels_number; l++) {
                rhs += (_mlcc_network->edgeCapacity(l) + 1) * _arc_vars[i][j][l];
            }
            _mp_solver->MakeRowConstraint(lhs >= rhs);
        }
    }

    for (int i=0; i < _vertex_count; i++) {
        if (i == _mlcc_network->center())
            continue;

        LinearExpr lhs = _flow_vars[i][_mlcc_network->center()];
        LinearExpr rhs;
        for (int l=0; l < _levels_number; l++) {
            rhs += _mlcc_network->edgeCapacity(l) * _arc_vars[i][_mlcc_network->center()][l];
        }
        _mp_solver->MakeRowConstraint(lhs <= rhs);
    }

    for (int i=0; i<_vertex_count; i++) {
        for (int j=0; j <_vertex_count; j++) {
            if (i==j || j == _mlcc_network->center())
                continue;

            LinearExpr lhs = _flow_vars[i][j];
            LinearExpr rhs =
                    (_mlcc_network->edgeCapacity(_levels_number-1) - _supply[j]) * _arc_vars[i][j][_levels_number-1];
            for (int l=0; l < _levels_number-1; l++) {
                rhs += _mlcc_network->edgeCapacity(l) * _arc_vars[i][j][l];
            }
            _mp_solver->MakeRowConstraint(lhs <= rhs);
        }
    }
}

}
