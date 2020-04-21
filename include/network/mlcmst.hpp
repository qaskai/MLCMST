#pragma once

#include <vector>

#include <network/mlcc_network.hpp>

namespace MLCMST::network {

class MLCMST final
{
public:
    MLCMST(const MLCCNetwork& network);
    ~MLCMST();

private:
    const MLCCNetwork& _network;
    std::vector<int> _parent;
    std::vector<int> _edge_level;

};

}
