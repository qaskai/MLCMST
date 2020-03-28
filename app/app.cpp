#include <iostream>

#include <model/network/capacitated_network.hpp>

int main(int argc, char const *argv[])
{
    std::cout << "Hello world" << std::endl;
    MLCMST::model::network::CapacitatedNetwork cn(4, {});
    return 0;
}