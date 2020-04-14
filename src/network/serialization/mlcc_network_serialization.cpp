#include <network/serialization/mlcc_network_serialization.hpp>

#include <vector>

#include <network/capacitated_network.hpp>
#include <network/serialization/fixed_size_network_serialization.hpp>

namespace MLCMST {
namespace network {
namespace serialization {


// ************ MLCCNetworkSerializer ************ //

/**
 * Format:
 * size center level_number
 * demands vector
 * capacity_level1
 * network_level1
 * capacity_level2
 * network_level2
 *
 **/
void MLCCNetworkSerializer::serialize(const MLCCNetwork& network, std::ostream& stream)
{
    stream << network.getSize() << " ";
    stream << network.getCenter() << " ";
    stream << network.getLevelsNumber() << "\n";

    for (int i=0; i<network.getSize(); i++) {
        stream << network.getDemand(i) << " ";
    }
    stream << "\n";

    FixedSizeNetworkSerializer network_serializer(network.getSize());
    for (int i=0; i<network.getLevelsNumber(); i++) {
        const CapacitatedNetwork& cap_net = network.getNetwork(i);
        stream << cap_net.getEdgeCapacity() << "\n";
        network_serializer.serialize(cap_net.getNetwork(), stream);
    }
}


// ************ MLCCNetworkDeserializer ************ //


MLCCNetwork MLCCNetworkDeserializer::deserialize(std::istream& stream)
{
    int size, center, level_number;
    std::vector<int> demands;
    std::vector<CapacitatedNetwork> networks;

    stream >> size >> center >> level_number;
    demands.reserve(size);
    networks.reserve(level_number);

    for (int i=0; i<size; i++) {
        int d;
        stream >> d;
        demands.push_back(d);
    }

    FixedSizeNetworkDeserializer network_deserializer(size);
    for (int i=0; i<level_number; i++) {
        int capacity;
        stream >> capacity;
        networks.emplace_back(capacity, network_deserializer.deserialize(stream));
    }

    return MLCCNetwork(center, networks, demands);
}

}
}
}
