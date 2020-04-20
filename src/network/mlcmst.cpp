#include <network/mlcmst.hpp>

namespace MLCMST {
namespace network {

MLCMST::MLCMST(const MLCCNetwork& network)
    : _network(network), _parent(network.vertexCount()), _edge_level(network.vertexCount())
{

}

MLCMST::~MLCMST() = default;

}
}
