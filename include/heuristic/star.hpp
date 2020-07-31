#pragma once

#include <string>

#include <heuristic/mlcmst_heuristic.hpp>

namespace MLCMST::heuristic {

class Star : public MLCMST_Heuristic
{
public:
    static std::string id();

    ~Star() override;

    network::MLCMST run(const network::MLCCNetwork &mlcc_network) override;
};

}
