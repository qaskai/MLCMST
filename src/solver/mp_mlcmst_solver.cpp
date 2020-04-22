#include <solver/mp_mlcmst_solver.hpp>

#include <mp/or_mip_solver.hpp>
#include <mp/or_lp_solver.hpp>

namespace MLCMST::solver {


MP_MLCMSTSolver::MP_MLCMSTSolver(bool exact_solution)
    : _mp_solver( exact_solution ?
        static_cast<std::unique_ptr<mp::MPSolver>>(std::make_unique<mp::ORMIPSolver>()) :
        std::make_unique<mp::ORLPSolver>()
    )
{

}

MP_MLCMSTSolver::MP_MLCMSTSolver(std::unique_ptr< mp::MPSolver > mp_solver) : _mp_solver(std::move(mp_solver))
{

}

MP_MLCMSTSolver::~MP_MLCMSTSolver() = default;

MLCMSTSolver::Result MP_MLCMSTSolver::solve(const network::MLCCNetwork &mlcc_network)
{
    _mp_solver->reset();

    setupLocalVariables(mlcc_network);
    createVariables();
    createConstraints();
    createObjective();

    _mp_solver->solve();

    bool finished = _mp_solver->resultStatus() == mp::MPSolver::OPTIMAL;
    return MLCMSTSolver::Result(
        finished && _mp_solver->isMIP() ? createMLCMST() : std::optional<network::MLCMST>(),
        finished ? _mp_solver->objectiveValue() : std::optional<double>(),
        _mp_solver->wallTime(),
        finished
    );
}

}
