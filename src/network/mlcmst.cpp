#include <network/mlcmst.hpp>

namespace MLCMST {
namespace network {

MLCMST::MLCMST(const MLCCNetwork& network)
    : _network(network), _parent(network.size()), _edge_level(network.size())
{

}

MLCMST::~MLCMST() = default;

}
}
