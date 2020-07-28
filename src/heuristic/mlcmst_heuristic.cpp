#include <heuristic/mlcmst_heuristic.hpp>

#include <chrono>

namespace MLCMST::heuristic {

MLCMST_Heuristic::~MLCMST_Heuristic() = default;


MLCMST_Solver::Result MLCMST_Heuristic::solve(const network::MLCCNetwork &mlcc_network)
{
    auto time_start = std::chrono::high_resolution_clock::now();

    network::MLCMST mlcmst = run(mlcc_network);

    auto time_end = std::chrono::high_resolution_clock::now();
    double wall_time = std::chrono::duration<double, std::milli>(time_end - time_start).count();

    return Result(
        mlcmst,
        std::nullopt,
        wall_time,
        true
    );
}

}
