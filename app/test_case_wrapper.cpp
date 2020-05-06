#include <iostream>
#include <vector>
#include <memory>
#include <map>
#include <functional>

#include <cxxopts.hpp>

#include <network/mlcc_network.hpp>
#include <network/serialization/mlcc_network_serialization.hpp>

#include <benchmark/test_case.hpp>
#include <benchmark/serialization/test_case_serialization.hpp>

#include <mp/mp_mlcmst_solver.hpp>
#include <mp/scf.hpp>
#include <mp/escf.hpp>
#include <mp/mcf.hpp>

#include "app.hpp"

using namespace MLCMST;

typedef MLCMST::network::MLCCNetwork MLCCNetwork;
typedef MLCMST::benchmark::TestCase TestCase;

struct Params
{
    bool optimal;
    std::string solver;
};

class TestCaseWrapperApp : public App<Params>
{
public:
    ~TestCaseWrapperApp() override;

private:
    const std::map<std::string, std::function<std::shared_ptr< mp::MP_MLCMSTSolver >(bool)> > solvers {
        std::make_pair("SCF", [] (bool optimal) { return std::make_shared<mp::SCF>(optimal); } ),
        std::make_pair("ESCF", [] (bool optimal) { return std::make_shared<mp::ESCF>(optimal); } ),
        std::make_pair("MCF", [] (bool optimal) { return std::make_shared<mp::MCF>(optimal); } )
    };

    cxxopts::Options createOptions() override;
    void validateParseResult(const cxxopts::ParseResult& result) override;
    Params extractParams(const cxxopts::ParseResult& result) override;
    void run(const Params& params) override;

    std::vector<MLCCNetwork> readNetworks();
    std::vector<TestCase> createTestCases(
            const std::vector<MLCCNetwork>& networks, const std::shared_ptr< mp::MP_MLCMSTSolver >& solver);
    void printTestCases(const std::vector<TestCase>& test_cases);
    std::shared_ptr< mp::MP_MLCMSTSolver > getSolver(const std::string& id, bool optimal);
};

TestCaseWrapperApp::~TestCaseWrapperApp() = default;

cxxopts::Options TestCaseWrapperApp::createOptions()
{
    std::string solver_option_string;
    for (const auto& p : solvers) {
        solver_option_string += p.first + ",";
    }

    cxxopts::Options options("TestCase wrapper", "MLCCNetwork to TestCase converter");

    options.add_options()
        ("optimal", "Lower bound with optimal solution", cxxopts::value<bool>()->default_value("false"))
        ("solver", "one of " + solver_option_string + " | required", cxxopts::value<std::string>())
        ("h,help", "Print usage")
        ;

    return options;
};


void TestCaseWrapperApp::validateParseResult(const cxxopts::ParseResult &result)
{
    if (!result.count("solver"))
        throw std::domain_error("solver argument is required");
    std::string solver_id = result["solver"].as<std::string>();
    if (getSolver(solver_id, false) == nullptr) {
        throw std::invalid_argument(solver_id + " is not a supported solver enum value");
    }
}

Params TestCaseWrapperApp::extractParams(const cxxopts::ParseResult &result)
{
    Params params{};
    params.optimal= result["optimal"].as<bool>();
    params.solver = result["solver"].as<std::string>();
    return params;
}

void TestCaseWrapperApp::run(const Params &params)
{
    std::vector<MLCCNetwork> networks = readNetworks();
    std::vector<TestCase> test_cases = createTestCases(networks, getSolver(params.solver, params.optimal));
    printTestCases(test_cases);
}

std::vector<MLCCNetwork> TestCaseWrapperApp::readNetworks()
{
    using MLCMST::network::serialization::MLCCNetworkDeserializer;

    unsigned N;
    std::cin >> N;

    MLCCNetworkDeserializer deserializer;
    std::vector<MLCCNetwork> networks;
    networks.reserve(N);
    while (N--) {
        networks.push_back(deserializer.deserialize(std::cin));
    }
    return networks;
}

std::vector<TestCase> TestCaseWrapperApp::createTestCases(
        const std::vector<MLCCNetwork> &networks, const std::shared_ptr<mp::MP_MLCMSTSolver>& solver)
{
    std::vector<TestCase> test_cases;
    test_cases.reserve(networks.size());
    for (const MLCCNetwork& n : networks) {
        double lower_bound = solver->solve(n).lower_bound.value();
        test_cases.emplace_back(lower_bound, n);
    }
    return test_cases;
}

void TestCaseWrapperApp::printTestCases(const std::vector<TestCase> &test_cases)
{
    using MLCMST::benchmark::serialization::TestCaseSerializer;

    std::cout << test_cases.size() << "\n\n";

    TestCaseSerializer serializer;
    for (const TestCase& test_case : test_cases) {
        serializer.serialize(test_case, std::cout);
        std::cout << "\n";
    }
}

std::shared_ptr<mp::MP_MLCMSTSolver> TestCaseWrapperApp::getSolver(const std::string &id, bool optimal)
{
    if (solvers.count(id)) {
        return solvers.at(id)(optimal);
    } else {
        return nullptr;
    }
}

int main(int argc, char** argv)
{
    TestCaseWrapperApp app;
    app.start(argc, argv);
    return 0;
}
