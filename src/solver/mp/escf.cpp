#include <solver/mp/escf.hpp>

namespace MLCMST::solver::mp {


ESCF::ESCF(bool exact_solution) : SCF(exact_solution)
{

}

ESCF::ESCF(std::unique_ptr<MLCMST::mp::MPSolver> mp_solver) : SCF(std::move(mp_solver))
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
    {
        const std::string constraint_name = "capacity1";
        _mp_solver->makeConstraintArray(_network_size, -_mp_solver->infinity(), 0, constraint_name);

        for (int i=0; i<_vertex_count; i++) {
            for (int j=0; j<_vertex_count; j++) {
                if (i==j)
                    continue;

                int edge_id = i*_vertex_count + j;
                _mp_solver->setConstraintCoefficient(-1, _flow_var_name, edge_id, constraint_name, edge_id);
                _mp_solver->setConstraintCoefficient(
                        1, _arc_var_name, 0*_network_size + edge_id, constraint_name, edge_id);
                for (int l=1; l < _levels_number; l++) {
                    _mp_solver->setConstraintCoefficient(
                        _mlcc_network->edgeCapacity(l-1) + 1,
                        _arc_var_name, l*_network_size + edge_id,
                        constraint_name, edge_id
                    );
                }
            }
        }
    }

    {
        const std::string constraint_name = "capacity2";
        _mp_solver->makeConstraintArray(_network_size, 0, _mp_solver->infinity(), constraint_name);

        for (int i=0; i < _vertex_count; i++) {
            if (i == _mlcc_network->center())
                continue;

            int edge_id = i*_vertex_count + _mlcc_network->center();
            _mp_solver->setConstraintCoefficient(-1, _flow_var_name, edge_id, constraint_name, edge_id);
            for (int l=0; l < _levels_number; l++) {
                _mp_solver->setConstraintCoefficient(
                    _mlcc_network->edgeCapacity(l),
                    _arc_var_name, l*_network_size + edge_id,
                    constraint_name, edge_id
                );
            }
        }

        for (int i=0; i<_vertex_count; i++) {
            for (int j=0; j <_vertex_count; j++) {
                if (i==j || j == _mlcc_network->center())
                    continue;

                int edge_id = i*_vertex_count + j;
                _mp_solver->setConstraintCoefficient(-1, _flow_var_name, edge_id, constraint_name, edge_id);
                _mp_solver->setConstraintCoefficient(
                    _mlcc_network->edgeCapacity(_levels_number-1) - _supply[j],
                    _arc_var_name, (_levels_number-1)*_network_size + edge_id,
                    constraint_name, edge_id
                );
                for (int l=0; l < _levels_number -1; l++) {
                    _mp_solver->setConstraintCoefficient(
                        _mlcc_network->edgeCapacity(l),
                        _arc_var_name, l*_network_size + edge_id,
                        constraint_name, edge_id
                    );
                }
            }
        }
    }
}

}
