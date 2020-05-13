#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>

#include <cxxopts.hpp>

#include <mlcmst_solver.hpp>
#include <mp/scf.hpp>
#include <mp/escf.hpp>
#include <mp/mcf.hpp>

#include <heuristic/link_upgrade_unit_demand.hpp>

#include <benchmark/test_case.hpp>
#include <benchmark/benchmark.hpp>
#include <benchmark/general_reporter.hpp>
#include <benchmark/serialization/test_case_serialization.hpp>

#include "app.hpp"

typedef MLCMST::benchmark::TestCase TestCase;
typedef MLCMST::benchmark::Benchmark Benchmark;
typedef MLCMST::benchmark::Reporter Reporter;
typedef MLCMST::benchmark::GeneralReporter GeneralReporter;


using namespace MLCMST;

struct Params
{
    std::vector<std::string> solvers;
};

class BenchmarkApp : public App<Params>
{
public:
    BenchmarkApp();
    ~BenchmarkApp() override;

private:
    const std::map<std::string, std::function<std::unique_ptr< MLCMST::MLCMSTSolver >()> > solvers {

    };

    const static std::map<int, int> m;

    cxxopts::Options createOptions() override;
    void validateParseResult(const cxxopts::ParseResult& result) override;
    Params extractParams(const cxxopts::ParseResult& result) override;
    void run(const Params& params) override;

    std::vector<TestCase> readTestCases(std::istream& in);
    Benchmark createBenchmark(std::unique_ptr< Reporter > reporter, const std::vector<std::string>& solver_ids,
            const std::vector<TestCase>& test_cases);
    std::unique_ptr< Reporter > createReporter();
};

BenchmarkApp::BenchmarkApp() : solvers({
    std::make_pair("SCF", [] () { return std::make_unique<MLCMST::mp::SCF>(false); } ),
    std::make_pair("ESCF", [] () { return std::make_unique<MLCMST::mp::ESCF>(false); } ),
    std::make_pair("MCF", [] () { return std::make_unique<MLCMST::mp::MCF>(false); } ),
    std::make_pair("link_upgrade_UD", [] () { return std::make_unique<heuristic::LinkUpgradeUnitDemand>(); })
})
{
}

BenchmarkApp::~BenchmarkApp() = default;

cxxopts::Options BenchmarkApp::createOptions()
{
    std::string solvers_list_string;
    for (const auto& p : solvers) {
        solvers_list_string += p.first + ",";
    }

    cxxopts::Options options("benchmark", "Runs set of test cases on the specified solver");

    options.add_options()
        ("solvers", "List of " + solvers_list_string + " | required", cxxopts::value<std::vector<std::string>>())
        ("h,help", "Print usage")
        ;

    return options;
}


void BenchmarkApp::validateParseResult(const cxxopts::ParseResult &result)
{
    const std::string argument_required_suffix = " argument is required";

    if (!result.count("solvers"))
        throw std::domain_error("solvers" + argument_required_suffix);
    std::vector<std::string> solver_ids = result["solvers"].as<std::vector<std::string>>();
    for (const std::string& id : solver_ids) {
        if (!solvers.count(id))
            throw std::invalid_argument(id + " is not a valid solver id");
    }
}


Params BenchmarkApp::extractParams(const cxxopts::ParseResult &result)
{
    Params params;
    params.solvers = result["solvers"].as<std::vector<std::string>>();
    return params;
}


void BenchmarkApp::run(const Params &params)
{
    std::vector<TestCase> test_cases = readTestCases(std::cin);
    Benchmark benchmark = createBenchmark(createReporter(), params.solvers, test_cases);
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

Benchmark BenchmarkApp::createBenchmark(std::unique_ptr< Reporter > reporter, const std::vector<std::string> &solver_ids,
        const std::vector<TestCase> &test_cases)
{
    Benchmark benchmark(std::move(reporter));
    for (const TestCase& test_case : test_cases) {
        benchmark.addTestCase(test_case);
    }
    for (const std::string& solver_id : solver_ids) {
        benchmark.addSolver(solvers.at(solver_id)(), solver_id);
    }
    return std::move(benchmark);
}

std::unique_ptr<Reporter> BenchmarkApp::createReporter()
{
    return std::make_unique<GeneralReporter>(std::cout);
}


int main(int argc, char** argv)
{
    BenchmarkApp app;
    app.start(argc, argv);
    return 0;
}
