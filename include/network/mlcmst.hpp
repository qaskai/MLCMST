#pragma once

#include <vector>

#include <network/mlcc_network.hpp>

namespace MLCMST {
namespace network {

class MLCMST
{
private:
    const MLCCNetwork& network;

    std::vector<int> parent;
    std::vector<int> edge_level;

public:
    MLCMST(const MLCCNetwork& network);
    ~MLCMST() = default;
};

}
}
