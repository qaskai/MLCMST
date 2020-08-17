#include <numeric>
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
#include <benchmark/latex_table_reporter.hpp>
#include <benchmark/serialization/test_case_serialization.hpp>

#include "../app.hpp"
#include "solver_builder.hpp"

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
    ~BenchmarkApp() override;

private:
    bool fileExistenceCheck(const std::string& path);
    cxxopts::Options createOptions() override;
    void validateParseResult(const cxxopts::ParseResult& result) override;
    Params extractParams(const cxxopts::ParseResult& result) override;
    void run(const Params& params) override;

    std::vector<TestCase> readTestCases(std::istream& in);
    std::vector<std::pair<std::string, std::unique_ptr< MLCMST_Solver >>> createSolvers(const rapidjson::Value& json);
    Benchmark createBenchmark(std::vector<std::unique_ptr< Reporter >> reporters,
            std::vector<std::pair<std::string, std::unique_ptr< MLCMST_Solver >>> solvers,
            const std::vector<TestCase>& test_cases);
    std::vector<std::unique_ptr< Reporter >> createReporters();
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
    std::string benchmark_help_string = R""""(
    --config parameter should be a path to a file containing JSON with a list of solvers with their parameters.
    The format of those JSON for each type of solver follows.
    {
        "id": solver_id
        "name": string - must be unique, optional - if not provided id will be used
        "params": solver_parameters
    }
    Params attribute describes the solver parameters. It is different for every solver.
    )"""";
    benchmark_help_string +=
        std::accumulate(SolverBuilder::solver_json_template.begin(), SolverBuilder::solver_json_template.end(),
        std::string("\n"),
            [](std::string s, const auto& p) {
                return std::move(s) + p.second;
            });
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
    Benchmark benchmark = createBenchmark(createReporters(), createSolvers(document), test_cases);
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

Benchmark BenchmarkApp::createBenchmark(std::vector<std::unique_ptr<Reporter>> reporters,
                                        std::vector<std::pair<std::string, std::unique_ptr<MLCMST_Solver>>> solvers,
                                        const std::vector<TestCase> &test_cases) {
    Benchmark benchmark(std::move(reporters));
    for (const TestCase& test_case : test_cases) {
        benchmark.addTestCase(test_case);
    }
    for (auto& [id, solver] : solvers) {
        benchmark.addSolver(std::move(solver), id);
    }

    return std::move(benchmark);
}

std::vector<std::unique_ptr<Reporter>> BenchmarkApp::createReporters()
{
    std::vector<std::unique_ptr<Reporter>> reporters;
    reporters.push_back(std::make_unique<GeneralReporter>(std::cout));
    reporters.push_back(std::make_unique<benchmark::LatexTableReporter>(std::cout));
    return std::move(reporters);
}

std::vector<std::pair<std::string, std::unique_ptr<MLCMST_Solver >>>
BenchmarkApp::createSolvers(const rapidjson::Value &json)
{
    std::vector<std::pair<std::string, std::unique_ptr< MLCMST_Solver >>> solvers_with_names;
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
