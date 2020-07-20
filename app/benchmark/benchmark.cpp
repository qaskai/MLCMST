#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <fstream>

#include <cxxopts.hpp>

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

#include <benchmark/test_case.hpp>
#include <benchmark/benchmark.hpp>
#include <benchmark/general_reporter.hpp>
#include <benchmark/serialization/test_case_serialization.hpp>

#include "../app.hpp"
#include "solver_builder.hpp"
#include "help.hpp"

typedef MLCMST::benchmark::TestCase TestCase;
typedef MLCMST::benchmark::Benchmark Benchmark;
typedef MLCMST::benchmark::Reporter Reporter;
typedef MLCMST::benchmark::GeneralReporter GeneralReporter;


using namespace MLCMST;

struct Params
{
     std::string configJSONPath;
};

class BenchmarkApp : public App<Params>
{
public:
//    BenchmarkApp();
    ~BenchmarkApp() override;

private:
    bool fileExistenceCheck(const std::string& path);
    cxxopts::Options createOptions() override;
    void validateParseResult(const cxxopts::ParseResult& result) override;
    Params extractParams(const cxxopts::ParseResult& result) override;
    void run(const Params& params) override;

    std::vector<TestCase> readTestCases(std::istream& in);
    std::vector<std::pair<std::string, std::unique_ptr< MLCMSTSolver >>> createSolvers(const rapidjson::Value& json);
    Benchmark createBenchmark(std::unique_ptr< Reporter > reporter,
            std::vector<std::pair<std::string, std::unique_ptr< MLCMSTSolver >>> solvers,
            const std::vector<TestCase>& test_cases);
    std::unique_ptr< Reporter > createReporter();
};

bool BenchmarkApp::fileExistenceCheck(const std::string &path)
{
    std::fstream s(path);
    return s.good();
}

BenchmarkApp::~BenchmarkApp() = default;


cxxopts::Options BenchmarkApp::createOptions()
{
    cxxopts::Options options("benchmark", "Runs set of test cases on the specified solver");
    options.custom_help(benchmark_help_string);

    options.add_options()
        ("config", "Path to json file with a list of solver specifications", cxxopts::value<std::string>())
        ("h,help", "Print usage")
        ;

    return options;
}


void BenchmarkApp::validateParseResult(const cxxopts::ParseResult &result)
{
    const std::string argument_required_suffix = " argument is required";

    if (!result.count("config"))
        throw std::domain_error("config" + argument_required_suffix);
    std::string configJSONPath = result["config"].as<std::string>();
    if (!fileExistenceCheck(configJSONPath))
        throw std::domain_error("file " + configJSONPath + " doesn't exist");
}


Params BenchmarkApp::extractParams(const cxxopts::ParseResult &result)
{
    Params params;
    params.configJSONPath = result["config"].as<std::string>();
    return params;
}

void BenchmarkApp::run(const Params &params)
{
    std::vector<TestCase> test_cases = readTestCases(std::cin);
    std::ifstream file_stream(params.configJSONPath);
    rapidjson::IStreamWrapper json_stream(file_stream);
    rapidjson::Document document;
    document.ParseStream(json_stream);
    Benchmark benchmark = createBenchmark(createReporter(), createSolvers(document), test_cases);
    benchmark.run();
}

std::vector<TestCase> BenchmarkApp::readTestCases(std::istream &in)
{
    unsigned N;
    in >> N;

    std::vector<TestCase> test_cases;
    test_cases.reserve(N);
    MLCMST::benchmark::serialization::TestCaseDeserializer deserializer;
    while (N--) {
        test_cases.push_back(deserializer.deserialize(in));
    }
    return test_cases;
}

Benchmark BenchmarkApp::createBenchmark(std::unique_ptr<Reporter> reporter,
                                        std::vector<std::pair<std::string, std::unique_ptr<MLCMSTSolver>>> solvers,
                                        const std::vector<TestCase> &test_cases) {
    Benchmark benchmark(std::move(reporter));
    for (const TestCase& test_case : test_cases) {
        benchmark.addTestCase(test_case);
    }
    for (auto& [id, solver] : solvers) {
        benchmark.addSolver(std::move(solver), id);
    }

    return std::move(benchmark);
}

std::unique_ptr<Reporter> BenchmarkApp::createReporter()
{
    return std::make_unique<GeneralReporter>(std::cout);
}

std::vector<std::pair<std::string, std::unique_ptr<MLCMSTSolver >>>
BenchmarkApp::createSolvers(const rapidjson::Value &json)
{
    std::vector<std::pair<std::string, std::unique_ptr< MLCMSTSolver >>> solvers_with_names;
    if (json.IsArray()) {
        for (const rapidjson::Value& v : json.GetArray()) {
            solvers_with_names.push_back(SolverBuilder::buildNamedSolver(v));
        }
    } else {
        solvers_with_names.push_back(SolverBuilder::buildNamedSolver(json));
    }
    return std::move(solvers_with_names);
}

int main(int argc, char** argv)
{
    BenchmarkApp app;
    app.start(argc, argv);
    return 0;
}
