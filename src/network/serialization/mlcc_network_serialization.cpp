#include <network/serialization/mlcc_network_serialization.hpp>

#include <vector>

#include <network/capacitated_network.hpp>
#include <network/serialization/fixed_size_network_serialization.hpp>

namespace MLCMST {
namespace network {
namespace serialization {


// ************ MLCCNetworkSerializer ************ //


MLCCNetworkSerializer::MLCCNetworkSerializer() = default;

MLCCNetworkSerializer::~MLCCNetworkSerializer() = default;

void MLCCNetworkSerializer::serialize(const MLCCNetwork& network, std::ostream& stream)
{
    stream << network.size() << " ";
    stream << network.center() << " ";
    stream << network.levelsNumber() << "\n";

    for (int i=0; i< network.size(); i++) {
        stream << network.demand(i) << " ";
    }
    stream << "\n";

    FixedSizeNetworkSerializer network_serializer(network.size());
    for (int i=0; i< network.levelsNumber(); i++) {
        const CapacitatedNetwork& cap_net = network.network(i);
        stream << cap_net.edgeCapacity() << "\n";
        network_serializer.serialize(cap_net.network(), stream);
    }
}


// ************ MLCCNetworkDeserializer ************ //


MLCCNetworkDeserializer::MLCCNetworkDeserializer() = default;

MLCCNetworkDeserializer::~MLCCNetworkDeserializer() = default;

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
