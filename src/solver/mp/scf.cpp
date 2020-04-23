#include <solver/mp/scf.hpp>

#include <numeric>

namespace MLCMST::solver::mp {

SCF::SCF(bool exact_solution) : MP_MLCMSTSolver(exact_solution)
{
}

SCF::SCF(std::unique_ptr<MLCMST::mp::MPSolver> mp_solver) : MP_MLCMSTSolver(std::move(mp_solver))
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
    _mp_solver->makeVariableArray( _levels_number * _network_size, 0, 1, _arc_var_name);
    _mp_solver->makeNumVariableArray(_network_size, 0.0, infinity, _flow_var_name);
}

void SCF::createObjective()
{
    _mp_solver->setMinimization();
    for (int i=0; i < _vertex_count; i++) {
        for (int j=0; j < _vertex_count; j++) {
            if (i == j)
                continue;
            int edge_idx = i*_vertex_count + j;
            for (int l=0; l < _levels_number; l++) {
                double cost = _mlcc_network->network(l).edgeCost(i, j);
                _mp_solver->setObjectiveCoefficient(cost, _arc_var_name, l*_network_size + edge_idx);
            }
        }
    }
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
    const std::string constraint_name = "demands";
    _mp_solver->makeConstraintArray(_vertex_count, constraint_name);
    for (int i=0; i < _vertex_count; i++) {
        int W = i == _mlcc_network->center() ? _supply[i] : -_supply[i];
        _mp_solver->setConstraintBounds(W, W, constraint_name, i);
        for (int j=0; j < _vertex_count; j++) {
            if (i == j)
                continue;
            _mp_solver->setConstraintCoefficient(1, _flow_var_name, j*_vertex_count + i, constraint_name, i);
        }
        for (int j=0; j < _vertex_count; j++) {
            if (i == j)
                continue;
            _mp_solver->setConstraintCoefficient(-1, _flow_var_name, i*_vertex_count + j, constraint_name, i);
        }
    }
}

void SCF::createCapacityConstraints()
{
    const std::string constraint_name = "capacity";
    _mp_solver->makeConstraintArray(_network_size, 0, _mp_solver->infinity(), constraint_name);
    for (int i=0; i < _vertex_count; i++) {
        for (int j=0; j < _vertex_count; j++) {
            if (i == j)
                continue;
            int edge_idx = i*_vertex_count + j;
            _mp_solver->setConstraintCoefficient(-1, _flow_var_name, edge_idx, constraint_name, edge_idx);
            for (int l=0; l < _levels_number; l++) {
                _mp_solver->setConstraintCoefficient(
                        _mlcc_network->edgeCapacity(l),
                        _arc_var_name, l*_network_size + edge_idx, constraint_name, edge_idx);
            }
        }
    }
}

void SCF::createOneOutgoingConstraints()
{
    const std::string constraint_name = "one_outgoing";
    _mp_solver->makeConstraintArray(_vertex_count, 1, 1, constraint_name);
    for (int i=0; i < _vertex_count; i++) {
        for (int j=0; j < _vertex_count; j++) {
            int edge_idx = i*_vertex_count + j;
            for (int l =0; l < _levels_number; l++) {
                _mp_solver->setConstraintCoefficient(1, _arc_var_name, l*_network_size + edge_idx, constraint_name, i);
            }
        }
    }
}

void SCF::createOneBetweenConstraints()
{
    const std::string constraint_name = "one_between";
    _mp_solver->makeConstraintArray(_network_size, -_mp_solver->infinity(), 1, constraint_name);
    for (int i=0; i < _vertex_count; i++) {
        for (int j = i+1; j < _vertex_count; j++) {

            int edge_idx = i*_vertex_count + j;
            int rev_edge_idx = j*_vertex_count + i;
            for (int l=0; l < _levels_number; l++) {
                _mp_solver->setConstraintCoefficient(1, _arc_var_name, l*_network_size + edge_idx, constraint_name, edge_idx);
                _mp_solver->setConstraintCoefficient(1, _arc_var_name, l*_network_size + rev_edge_idx, constraint_name, edge_idx);
            }
        }
    }
}

network::MLCMST SCF::createMLCMST()
{
    std::vector<int> parents(_vertex_count), edge_level(_vertex_count);
    for (int i=0; i < _vertex_count; i++) {
        for (int l=0; l < _levels_number; l++) {
            for (int j=0; j < _vertex_count; j++) {
                if (_mp_solver->variableValue(_arc_var_name, l*_network_size + i*_vertex_count + j) > 0.99) {
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
