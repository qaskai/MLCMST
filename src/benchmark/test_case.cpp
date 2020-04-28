#include <benchmark/test_case.hpp>

namespace MLCMST::benchmark {

TestCase::TestCase(const network::MLCCNetwork& mlcc_network, double lower_bound)
    : _mlcc_network(mlcc_network), _lower_bound(lower_bound)
{

}

const network::MLCCNetwork& TestCase::mlccNetwork() const
{
    return _mlcc_network;
}

double TestCase::lowerBound() const
{
    return _lower_bound;
}

}
