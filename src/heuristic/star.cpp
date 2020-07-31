#include <heuristic/star.hpp>

namespace MLCMST::heuristic {

std::string Star::id()
{
    static const std::string id = "star";
    return id;
}

Star::~Star() = default;

network::MLCMST Star::run(const network::MLCCNetwork &mlcc_network)
{
    return network::MLCMST::star(mlcc_network);
}

}
