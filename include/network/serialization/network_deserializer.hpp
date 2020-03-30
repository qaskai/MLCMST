#pragma once

#include <istream>

#include <network/network.hpp>
#include <util/deserializer.hpp>


namespace MLCMST {
namespace network {
namespace serialization {

class NetworkDeserializer : public util::Deserializer<Network>
{
public:
    NetworkDeserializer();
    ~NetworkDeserializer();
    
    Network deserialize(std::istream& stream);
    
};

}
}
}
