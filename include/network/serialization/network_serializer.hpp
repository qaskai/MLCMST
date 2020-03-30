#pragma once

#include <ostream>

#include <network/network.hpp>
#include <util/serializer.hpp>


namespace MLCMST {
namespace network {
namespace serialization {

class NetworkSerializer : public util::Serializer<Network>
{
public:
    NetworkSerializer();
    ~NetworkSerializer();
    
    void serialize(const Network& network, std::ostream& stream);

};

}
}
}
