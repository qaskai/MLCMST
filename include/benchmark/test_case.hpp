#pragma once

#include <optional>

#include <network/mlcc_network.hpp>
#include <network/mlcmst.hpp>

namespace MLCMST::benchmark {

class TestCase
{
public:
    TestCase(const network::MLCCNetwork& mlcc_network, double lower_bound);

    [[nodiscard]] const network::MLCCNetwork& mlccNetwork() const;
    [[nodiscard]] double lowerBound() const;

private:
    network::MLCCNetwork _mlcc_network;
    double _lower_bound;
};

}
