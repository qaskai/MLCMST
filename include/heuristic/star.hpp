#pragma once

#include <heuristic/mlcmst_heuristic.hpp>

namespace MLCMST::heuristic {

class Star : public MLCMST_Heuristic
{
public:
    ~Star() override;

    network::MLCMST run(const network::MLCCNetwork &mlcc_network) override;
};

}
