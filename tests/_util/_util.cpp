#include <_util.hpp>

#include <network/generation/euclid_mlcc_network_generator.hpp>
#include <geometry/generation/int_point_generator.hpp>

namespace MLCMST::tests {


network::MLCCNetwork Util::generateNetwork(int size, bool unit_demand, long seed)
{
    using network::generation::EuclidMLCCNetworkGenerator;
    using Level = EuclidMLCCNetworkGenerator::Level;
    using CenterPosition = EuclidMLCCNetworkGenerator::CenterPosition;

    int k =1;
    while (k*k < size*4) {
        k++;
    }
    std::vector<Level> levels = {
            Level{1,1}, Level{3,2}, Level {10, 6}};
    CenterPosition centerPosition = CenterPosition ::CENTER;
    const int max_demand = unit_demand ? 1 : 3;

    EuclidMLCCNetworkGenerator generator(size, centerPosition, max_demand, levels,
            std::move(std::make_unique<geometry::generation::IntPointGenerator>(0, k, seed)),
            seed);

    return generator.generate();
}

}
