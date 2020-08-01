#include <heuristic/grasp.hpp>

namespace MLCMST::heuristic {

std::string GRASP::id()
{
    static const std::string id = "GRASP";
    return id;
}

GRASP::GRASP(std::unique_ptr<MLCMST_Heuristic> construction_phase, std::unique_ptr<improvement::MLCMST_Improver> improvement_phase,
        Params params)
    : params(params), construction_phase_(std::move(construction_phase)), improvement_phase_(std::move(improvement_phase))
{
}

GRASP::~GRASP() = default;

network::MLCMST GRASP::run(const network::MLCCNetwork &mlcc_network)
{
    struct Solution {
        network::MLCMST mlcmst;
        double cost;
    };

    Solution best_solution { .mlcmst=network::MLCMST(0,0), .cost=std::numeric_limits<double>::max() };
    for (int i=0; i<params.iterations_no; i++) {
        network::MLCMST mlcmst = construction_phase_->run(mlcc_network);
        mlcmst = improvement_phase_->improve(mlcmst, mlcc_network);

        double cost = mlcmst.cost(mlcc_network);
        if (best_solution.cost > cost) {
            best_solution = Solution{ mlcmst, cost };
        }
    }
    return best_solution.mlcmst;
}

}
