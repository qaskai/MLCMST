#include <heuristic/star.hpp>

namespace MLCMST::heuristic {

Star::~Star() = default;

network::MLCMST Star::run(const network::MLCCNetwork &mlcc_network)
{
    return network::MLCMST::star(mlcc_network);
}

}
