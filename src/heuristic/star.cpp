#include <heuristic/star.hpp>

namespace MLCMST::heuristic {

std::string Star::id()
{
    static const std::string id = "star";
    return id;
}

Star::~Star() = default;

network::MLCST Star::run(const network::MLCCNetwork &mlcc_network)
{
    return network::MLCST::star(mlcc_network);
}

}
