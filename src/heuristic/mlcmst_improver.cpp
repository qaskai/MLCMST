#include <heuristic/mlcmst_improver.hpp>

#include <heuristic/star.hpp>

namespace MLCMST::heuristic {

MLCMST_Improver::~MLCMST_Improver() = default;

MLCMST_Improver::MLCMST_Improver() : MLCMST_Improver(std::make_unique<Star>())
{
}

MLCMST_Improver::MLCMST_Improver(std::unique_ptr<MLCMST_Solver> init_solver) : init_solver_(std::move(init_solver))
{
}

network::MLCMST MLCMST_Improver::run(const network::MLCCNetwork &mlcc_network)
{
    network::MLCMST mlcmst = network::MLCMST::star(mlcc_network);
    mlcmst = improve(mlcmst, mlcc_network);
    return mlcmst;
}

}
