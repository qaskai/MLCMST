#include <mp/mp_mlcmst_solver.hpp>

#include <chrono>

namespace MLCMST::mp {


MP_MLCMSTSolver::MP_MLCMSTSolver(bool exact_solution) : _mp_solver( mp::MPSolverFactory(exact_solution).create() )
{
}

MP_MLCMSTSolver::MP_MLCMSTSolver(mp::MPSolverFactory mp_solver_factory) : _mp_solver(mp_solver_factory.create())
{
}

MP_MLCMSTSolver::~MP_MLCMSTSolver() = default;

bool MP_MLCMSTSolver::setThreadNum(int thread_num)
{
    auto status = _mp_solver.SetNumThreads(thread_num);
    return status == absl::OkStatus();
}

MLCMSTSolver::Result MP_MLCMSTSolver::solve(const network::MLCCNetwork &mlcc_network)
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
    return MLCMSTSolver::Result(
        finished && _mp_solver.IsMIP() ? createMLCMST() : std::optional<network::MLCMST>(),
        finished ? _mp_solver.Objective().Value() : std::optional<double>(),
        wall_time,
        finished
    );
}

}
