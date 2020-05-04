#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
#include <optional>
#include <map>

#include <cxxopts.hpp>

#include <network/mlcc_network.hpp>
#include <network/generation/euclid_mlcc_network_generator.hpp>
#include <network/serialization/mlcc_network_serialization.hpp>

#include <geometry/generation/real_point_generator.hpp>
#include <geometry/generation/int_point_generator.hpp>

typedef MLCMST::network::MLCCNetwork MLCCNetwork;
typedef MLCMST::network::generation::EuclidMLCCNetworkGenerator EuclidMLCCNetworkGenerator;
typedef EuclidMLCCNetworkGenerator::CenterPosition CenterPosition;
typedef EuclidMLCCNetworkGenerator::Level Level;



struct GenerationParams
{
    unsigned number;
    unsigned size;
    double from, to;
    bool integer;
    CenterPosition center_position;
    std::vector<Level> levels;
};

cxxopts::Options createOptionsObject();
GenerationParams extractParams(const cxxopts::ParseResult& result);
void validateParseResult(const cxxopts::ParseResult& result);
std::vector<MLCCNetwork> generateNetworks(const GenerationParams& params);
void printNetworks(const std::vector<MLCCNetwork>& networks);
std::optional<CenterPosition> getCenterPosition(const std::string& id);

int main(int argc, char** argv)
{
    cxxopts::Options options = createOptionsObject();
    cxxopts::ParseResult result = options.parse(argc, argv);
    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        return 0;
    }
    validateParseResult(result);
    GenerationParams params = extractParams(result);

    auto networks = generateNetworks(params);
    printNetworks(networks);

    return 0;
}

cxxopts::Options createOptionsObject()
{
    cxxopts::Options options("generator", "Random Euclid MLCC Network generator");

    options.add_options()
        ("n,number", "Number of instances generated", cxxopts::value<unsigned>())
        ("size", "Graph size", cxxopts::value<unsigned>())
        ("range", "Points in range", cxxopts::value<std::vector<double>>())
        ("integer", "Points are on integer grid", cxxopts::value<bool>()->default_value("false"))
        ("capacity", "Graph levels edge capacity", cxxopts::value<std::vector<unsigned>>())
        ("cost", "Graph levels cost multiplier", cxxopts::value<std::vector<double>>())
        ("center", "enum: RANDOM | CORNER | CENTER", cxxopts::value<std::string>())
        ("h,help", "Print usage")
    ;

    return options;
}


GenerationParams extractParams(const cxxopts::ParseResult& result)
{
    GenerationParams params;
    params.number = result["number"].as<unsigned>();
    params.size = result["size"].as<unsigned>();
    params.integer = result["integer"].as<bool>();
    params.center_position = getCenterPosition(result["center"].as<std::string>()).value();

    std::vector<double> range = result["range"].as<std::vector<double>>();
    params.from = range[0];
    params.to = range[1];

    std::vector<unsigned> level_capacity = result["capacity"].as<std::vector<unsigned>>();
    std::vector<double> cost_multiplier = result["cost"].as<std::vector<double>>();
    params.levels.resize(cost_multiplier.size());
    for (int i=0; i<cost_multiplier.size(); i++) {
        params.levels[i] = Level{ level_capacity[i], cost_multiplier[i] };
    }
    return params;
}

void validateParseResult(const cxxopts::ParseResult& result)
{
    result["number"].as<unsigned>();
    result["size"].as<unsigned>();
    result["integer"].as<bool>();
    std::string center = result["center"].as<std::string>();
    if (!getCenterPosition(center).has_value()) {
        throw std::logic_error(center + " is not a valid center enum");
    }
    std::vector<double> range = result["range"].as<std::vector<double>>();
    if (range.size() != 2) {
        throw std::logic_error("range should contain exactly two numbers");
    }
    if (range[0] >= range[1]) {
        throw std::logic_error("first element of range should be smaller then the second");
    }

    std::vector<unsigned> level_capacity = result["capacity"].as<std::vector<unsigned>>();
    std::vector<double> cost_multiplier = result["cost"].as<std::vector<double>>();
    if (level_capacity.size() != cost_multiplier.size()) {
        throw std::logic_error("capacity and cost should have equal lengths");
    }
}

std::vector<MLCCNetwork> generateNetworks(const GenerationParams& params)
{
    using namespace MLCMST::geometry;
    using MLCMST::Generator;

    std::unique_ptr< Generator<Point> > point_generator;
    if (params.integer)
        point_generator = std::make_unique<generation::IntPointGenerator>(params.from, params.to);
    else
        point_generator = std::make_unique<generation::RealPointGenerator>(params.from, params.to);

    EuclidMLCCNetworkGenerator generator(params.size, params.center_position, params.levels, std::move(point_generator));

    std::vector<MLCCNetwork> networks;
    while (networks.size() < params.number) {
        networks.push_back(generator.generate());
    }
    return networks;
}

void printNetworks(const std::vector<MLCCNetwork>& networks)
{
    using MLCMST::network::serialization::MLCCNetworkSerializer;
    MLCCNetworkSerializer serializer;

    std::cout << networks.size() << "\n\n";
    for (const MLCCNetwork& n : networks) {
        serializer.serialize(n, std::cout);
        std::cout << "\n";
    }
}

std::optional<CenterPosition> getCenterPosition(const std::string& id)
{
    std::map<std::string, CenterPosition> id_to_enum {
        std::make_pair("CENTER", CenterPosition::CENTER),
        std::make_pair("CORNER", CenterPosition::CORNER),
        std::make_pair("RANDOM", CenterPosition::RANDOM)
    };
    if (id_to_enum.count(id))
        return id_to_enum[id];
    else
        return std::nullopt;
}

