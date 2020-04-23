#include <solver/mp_mlcmst_solver.hpp>

#include <chrono>

namespace MLCMST::solver {


MP_MLCMSTSolver::MP_MLCMSTSolver(bool exact_solution) : _mp_solver_factory( mp::MPSolverFactory(exact_solution) )
{
}

MP_MLCMSTSolver::MP_MLCMSTSolver(mp::MPSolverFactory mp_solver_factory) : _mp_solver_factory(mp_solver_factory)
{
}

MP_MLCMSTSolver::~MP_MLCMSTSolver() = default;

MLCMSTSolver::Result MP_MLCMSTSolver::solve(const network::MLCCNetwork &mlcc_network)
{
    _mp_solver = _mp_solver_factory.create();

    setupLocalVariables(mlcc_network);
    createVariables();
    createConstraints();
    createObjective();

    auto time_start = std::chrono::high_resolution_clock::now();
    auto result_status = _mp_solver->Solve();
    auto time_end = std::chrono::high_resolution_clock::now();
    double wall_time = std::chrono::duration<double, std::milli>(time_end - time_start).count();

    bool finished = result_status == MPSolver::OPTIMAL;
    return MLCMSTSolver::Result(
        finished && _mp_solver->IsMIP() ? createMLCMST() : std::optional<network::MLCMST>(),
        finished ? _mp_solver->Objective().Value() : std::optional<double>(),
        wall_time,
        finished
    );
}

}
