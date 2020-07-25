#include <_util.hpp>

#include <network/generation/euclid_mlcc_network_generator.hpp>
#include <geometry/generation/int_point_generator.hpp>

namespace MLCMST::tests {


network::MLCCNetwork Util::generateNetwork(int size, bool unit_demand, long seed)
{
    using network::generation::EuclidMLCCNetworkGenerator;
    using Level = EuclidMLCCNetworkGenerator::Level;
    using CenterPosition = EuclidMLCCNetworkGenerator::CenterPosition;
    using DemandType = EuclidMLCCNetworkGenerator::DemandType;

    int k =1;
    while (k*k < size*4) {
        k++;
    }
    std::vector<Level> levels = {
            Level{1,1}, Level{3,2}, Level {10, 5}};
    CenterPosition centerPosition = CenterPosition ::CENTER;
    DemandType demandType = unit_demand ? DemandType ::UNIT : DemandType ::RANDOM;

    EuclidMLCCNetworkGenerator generator(size, centerPosition, demandType, levels,
            std::move(std::make_unique<geometry::generation::IntPointGenerator>(0, k, seed)));
    generator.setMaxRandomDemand(3);

    return generator.generate();
}

}
