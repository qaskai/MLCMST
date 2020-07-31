#pragma once

#include <memory>

#include <mlcmst_solver.hpp>
#include <heuristic/mlcmst_heuristic.hpp>

#include <network/mlcmst.hpp>
#include <network/mlcc_network.hpp>

namespace MLCMST::heuristic {

class MLCMST_Improver : public MLCMST_Heuristic
{
public:
    MLCMST_Improver();
    explicit MLCMST_Improver(std::unique_ptr< MLCMST_Solver > init_solver);
    ~MLCMST_Improver() override;

    network::MLCMST run(const network::MLCCNetwork &mlcc_network) final;

    virtual network::MLCMST improve(network::MLCMST mlcmst, const network::MLCCNetwork& mlcc_network) = 0;

private:
    std::unique_ptr< MLCMST_Solver > init_solver_;

};

}
