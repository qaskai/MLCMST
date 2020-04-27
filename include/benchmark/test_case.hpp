#pragma once

#include <optional>

#include <network/mlcc_network.hpp>
#include <network/mlcmst.hpp>

namespace MLCMST::benchmark {

class TestCase
{
public:
    explicit TestCase(const network::MLCCNetwork& mlcc_network);
    TestCase(const network::MLCCNetwork& mlcc_network, const network::MLCMST& mlcmst);

private:
    network::MLCCNetwork _mlcc_network;
    std::optional< network::MLCMST > _mlcmst;
};

}
