#include <benchmark/test_case.hpp>

namespace MLCMST::benchmark {

TestCase::TestCase(const network::MLCCNetwork& mlcc_network) : _mlcc_network(mlcc_network), _mlcmst(std::nullopt)
{

}

TestCase::TestCase(const network::MLCCNetwork& mlcc_network, const network::MLCMST& mlcmst)
    : _mlcc_network(mlcc_network), _mlcmst(mlcmst)
{

}

const network::MLCCNetwork& TestCase::mlccNetwork() const
{
    return _mlcc_network;
}

const std::optional<network::MLCMST>& TestCase::mlcmst() const
{
    return _mlcmst;
}


}
