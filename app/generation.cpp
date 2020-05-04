#include <iostream>

#include <cxxopts.hpp>

cxxopts::Options createOptionsObject()
{
    cxxopts::Options options("generator", "Random Euclid MLCC Network generator");

    options.add_options()
        ("n,size", "Graph size", cxxopts::value<unsigned>())
        ("h,help", "Print usage")
        ;

    return options;
}


int main(int argc, char** argv)
{
    cxxopts::Options options = createOptionsObject();
    cxxopts::ParseResult result = options.parse(argc, argv);

    if (result.count("help")) {
        std::cout << options.help() << std::endl;
    }

    return 0;
}