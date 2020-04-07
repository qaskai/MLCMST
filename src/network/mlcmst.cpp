#include <network/mlcmst.hpp>

namespace MLCMST {
namespace network {

MLCMST::MLCMST(const MLCCNetwork& network)
: network(network), parent(network.getSize()), edge_level(network.getSize())
{

}

}
}
