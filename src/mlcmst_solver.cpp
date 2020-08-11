#include <mlcmst_solver.hpp>
#include <utility>

namespace MLCMST {

MLCMST_Solver::~MLCMST_Solver() = default;

MLCMST_Solver::Result::Result(
        std::optional<network::MLCST> mlcmst, std::optional<double> lower_bound, double wall_time, bool finished
)
    : mlcmst(std::move(mlcmst)), lower_bound(std::move(lower_bound)), wall_time(wall_time), finished(finished)
{
}

}
