#include <benchmark/test_case.hpp>

namespace MLCMST::benchmark {

TestCase::TestCase(double lower_bound, const network::MLCCNetwork &mlcc_network)
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
