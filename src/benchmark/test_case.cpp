#include <benchmark/test_case.hpp>

namespace MLCMST::benchmark {

TestCase::TestCase(const network::MLCCNetwork& mlcc_network) : _mlcc_network(mlcc_network)
{

}

TestCase::TestCase(const network::MLCCNetwork& mlcc_network, const network::MLCMST& mlcmst)
    : _mlcc_network(mlcc_network), _mlcmst(mlcmst)
{

}


}
