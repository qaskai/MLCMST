#include <mp/mlcmst_mp_solver.hpp>

#include <chrono>

namespace MLCMST::mp {


MLCMST_MPSolver::MLCMST_MPSolver(bool exact_solution) : _mp_solver(mp::MPSolverFactory(exact_solution).create() )
{
}

MLCMST_MPSolver::MLCMST_MPSolver(mp::MPSolverFactory mp_solver_factory) : _mp_solver(mp_solver_factory.create())
{
}

MLCMST_MPSolver::~MLCMST_MPSolver() = default;

bool MLCMST_MPSolver::setThreadNum(int thread_num)
{
    auto status = _mp_solver.SetNumThreads(thread_num);
    return status == absl::OkStatus();
}

MLCMST_Solver::Result MLCMST_MPSolver::solve(const network::MLCCNetwork &mlcc_network)
{
    _mp_solver.Clear();
    _mlcc_network = &mlcc_network;

    setupLocalVariables();
    createVariables();
    createConstraints();
    createObjective();

    auto time_start = std::chrono::high_resolution_clock::now();
    auto result_status = _mp_solver.Solve();
    auto time_end = std::chrono::high_resolution_clock::now();
    double wall_time = std::chrono::duration<double, std::milli>(time_end - time_start).count();

    bool finished = result_status == MPSolver::OPTIMAL;
    return MLCMST_Solver::Result(
        finished && _mp_solver.IsMIP() ? createMLCMST() : std::optional<network::MLCMST>(),
        finished ? _mp_solver.Objective().Value() : std::optional<double>(),
        wall_time,
        finished
    );
}

}
