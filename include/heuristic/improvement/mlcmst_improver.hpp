#pragma once

#include <memory>

#include <mlcmst_solver.hpp>
#include <heuristic/mlcmst_heuristic.hpp>

#include <network/mlcst.hpp>
#include <network/mlcc_network.hpp>

namespace MLCMST::heuristic::improvement {

class MLCMST_Improver : public MLCMST_Heuristic
{
public:
    MLCMST_Improver();
    explicit MLCMST_Improver(std::unique_ptr< MLCMST_Solver > init_solver);
    ~MLCMST_Improver() override;

    network::MLCST run(const network::MLCCNetwork &mlcc_network) final;

    network::MLCST improve(const network::MLCST& mlcmst, const network::MLCCNetwork& mlcc_network);
    virtual network::MLCST improve(long steps, network::MLCST mlcmst, const network::MLCCNetwork& mlcc_network) = 0;

private:
    std::unique_ptr< MLCMST_Solver > init_solver_;

};

}
