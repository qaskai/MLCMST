#pragma once

#include <optional>

#include <network/mlcc_network.hpp>
#include <network/mlcst.hpp>

namespace MLCMST::benchmark {

class TestCase
{
public:
    TestCase(double lower_bound, const network::MLCCNetwork &mlcc_network);

    [[nodiscard]] const network::MLCCNetwork& mlccNetwork() const;
    [[nodiscard]] double lowerBound() const;

private:
    network::MLCCNetwork _mlcc_network;
    double _lower_bound;
};

}
