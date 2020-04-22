#include <solver/mlcmst_solver.hpp>
#include <utility>

namespace MLCMST::solver {

MLCMSTSolver::~MLCMSTSolver() = default;

MLCMSTSolver::Result::Result(
        std::optional<network::MLCMST> mlcmst, std::optional<double> lower_bound, double wall_time, bool finished
)
    : mlcmst(std::move(mlcmst)), lower_bound(std::move(lower_bound)), wall_time(wall_time), finished(finished)
{
}

}
