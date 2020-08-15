#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
#include <sstream>
#include <map>

#include <cxxopts.hpp>

#include <network/mlcc_network.hpp>
#include <network/generation/euclid_mlcc_network_generator.hpp>
#include <network/serialization/mlcc_network_serialization.hpp>

#include <geometry/generation/real_point_generator.hpp>
#include <geometry/generation/int_point_generator.hpp>
#include <geometry/serialization/point_set_serialization.hpp>

#include <util/util.hpp>

#include "app.hpp"

typedef MLCMST::network::MLCCNetwork MLCCNetwork;
typedef MLCMST::network::generation::EuclidMLCCNetworkGenerator EuclidMLCCNetworkGenerator;
typedef EuclidMLCCNetworkGenerator::CenterPosition CenterPosition;
typedef EuclidMLCCNetworkGenerator::Level Level;



struct GenerationParams
{
    unsigned number;
    unsigned size;
    int max_demand;
    CenterPosition center_position;
    std::vector<Level> levels;
    double from, to;
    bool integer;
    long seed;
    bool print_points;
};

class GenerationApp : public App<GenerationParams>
{
public:
    ~GenerationApp() override;

private:
    const std::map<std::string, CenterPosition> center_positions {
        std::make_pair("CENTER", CenterPosition::CENTER),
        std::make_pair("CORNER", CenterPosition::CORNER),
        std::make_pair("RANDOM", CenterPosition::RANDOM)
    };

    std::string suffix;

    cxxopts::Options createOptions() override;
    void validateParseResult(const cxxopts::ParseResult& result) override;
    GenerationParams extractParams(const cxxopts::ParseResult& result) override;
    void run (const GenerationParams& params) override;

    std::vector<MLCCNetwork> generateNetworks(const GenerationParams& params);
    void printNetworks(const std::vector<MLCCNetwork>& networks);
};

GenerationApp::~GenerationApp() = default;

cxxopts::Options GenerationApp::createOptions()
{
    std::string center_positions_string;
    for (const auto& p : center_positions) {
        center_positions_string += p.first + ",";
    }

    cxxopts::Options options("generator", "Random Euclid MLCC Network generator");

    options.add_options()
        ("n,number", "Number of instances generated | required", cxxopts::value<unsigned>())
        ("size", "Graph size | required", cxxopts::value<unsigned>())
        ("range", "Points in range | required", cxxopts::value<std::vector<double>>())
        ("integer", "Points are on integer grid", cxxopts::value<bool>()->default_value("false"))
        ("capacity", "Graph levels edge capacity | required", cxxopts::value<std::vector<unsigned>>())
        ("cost", "Graph levels cost multiplier | required", cxxopts::value<std::vector<double>>())
        ("center", "enum: " + center_positions_string + " | required", cxxopts::value<std::string>())
        ("max_demand", "Node demand limit. Terminal demands are random integer from [1,max_demand]", cxxopts::value<unsigned>()->default_value("1"))
        ("seed", "seed used in random points generator", cxxopts::value<long>())
        ("print_points", "Prepend network with underling node 2D point sets", cxxopts::value<bool>()->default_value("false"))
        ("h,help", "Print usage")
        ;

    return options;
}

void GenerationApp::validateParseResult(const cxxopts::ParseResult &result)
{
    const std::string required_arg = " argument is required";

    if (!result.count("number"))
        throw std::domain_error("number" + required_arg);
    result["number"].as<unsigned>();
    if (!result.count("size")) {
        throw std::domain_error("size" + required_arg);
    }
    result["size"].as<unsigned>();
    result["integer"].as<bool>();

    if (!result.count("center"))
        throw std::domain_error("center" + required_arg);
    std::string center = result["center"].as<std::string>();
    if (!center_positions.count(center)) {
        throw std::invalid_argument(center + " is not a valid center enum");
    }

    if (!result.count("range"))
        throw std::domain_error("range" + required_arg);
    std::vector<double> range = result["range"].as<std::vector<double>>();
    if (range.size() != 2) {
        throw std::invalid_argument("range should contain exactly two numbers");
    }
    if (range[0] >= range[1]) {
        throw std::invalid_argument("first element of range should be smaller then the second");
    }

    if (!result["capacity"].count() || !result.count("cost"))
        throw std::domain_error("capacity and cost " + required_arg);
    std::vector<unsigned> level_capacity = result["capacity"].as<std::vector<unsigned>>();
    std::vector<double> cost_multiplier = result["cost"].as<std::vector<double>>();
    if (level_capacity.size() != cost_multiplier.size()) {
        throw std::invalid_argument("capacity and cost should have equal lengths");
    }
}


GenerationParams GenerationApp::extractParams(const cxxopts::ParseResult &result)
{
    GenerationParams params;
    params.number = result["number"].as<unsigned>();
    params.size = result["size"].as<unsigned>();
    params.integer = result["integer"].as<bool>();
    params.max_demand = result["max_demand"].as<unsigned>();
    if (result.count("seed")) {
        params.seed = result["seed"].as<long>();
    } else {
        params.seed = MLCMST::util::clockMilliseconds();
    }
    params.print_points = result["print_points"].as<bool>();
    params.center_position = center_positions.at(result["center"].as<std::string>());

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

void GenerationApp::run(const GenerationParams& params)
{
    auto networks = generateNetworks(params);
    printNetworks(networks);
    if (params.print_points) {
        std::cout << suffix;
    }
}

std::vector<MLCCNetwork> GenerationApp::generateNetworks(const GenerationParams &params)
{
    using namespace MLCMST::geometry;
    using MLCMST::Generator;

    const bool print_points = params.print_points;

    std::unique_ptr< Generator<Point> > point_generator;
    if (params.integer)
        point_generator = std::make_unique<generation::IntPointGenerator>(params.from, params.to, params.seed);
    else
        point_generator = std::make_unique<generation::RealPointGenerator>(params.from, params.to, params.seed);

    EuclidMLCCNetworkGenerator generator(
        params.size, params.center_position, params.max_demand, params.levels, std::move(point_generator), params.seed);

    std::vector<MLCCNetwork> networks;
    std::stringstream points_info;
    points_info << params.number << "\n\n";

    while (networks.size() < params.number) {
        networks.push_back(generator.generate());
        MLCMST::geometry::serialization::PointSetSerializer().serialize(generator.lastPointSet(), points_info);
        points_info << "\n";
    }
    suffix = points_info.str();
    return networks;
}

void GenerationApp::printNetworks(const std::vector<MLCCNetwork> &networks)
{
    using MLCMST::network::serialization::MLCCNetworkSerializer;
    MLCCNetworkSerializer serializer;

    std::cout << networks.size() << "\n\n";
    for (const MLCCNetwork& n : networks) {
        serializer.serialize(n, std::cout);
        std::cout << "\n";
    }
}


int main(int argc, char** argv)
{
    GenerationApp app;
    app.start(argc, argv);
    return 0;
}
