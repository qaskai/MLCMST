#include <heuristic/improvement/mlcmst_improver.hpp>

#include <heuristic/star.hpp>

namespace MLCMST::heuristic::improvement {

MLCMST_Improver::~MLCMST_Improver() = default;

MLCMST_Improver::MLCMST_Improver() : MLCMST_Improver(std::make_unique<Star>())
{
}

MLCMST_Improver::MLCMST_Improver(std::unique_ptr<MLCMST_Solver> init_solver) : init_solver_(std::move(init_solver))
{
}

network::MLCMST MLCMST_Improver::run(const network::MLCCNetwork &mlcc_network)
{
    network::MLCMST mlcmst = init_solver_->solve(mlcc_network).mlcmst.value();
    mlcmst = improve(mlcmst, mlcc_network);
    return mlcmst;
}

network::MLCMST MLCMST_Improver::improve(const network::MLCMST& mlcmst, const network::MLCCNetwork &mlcc_network)
{
    return improve(std::numeric_limits<long>::max(), mlcmst, mlcc_network);
}

}
