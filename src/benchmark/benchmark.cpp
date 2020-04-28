#include <benchmark/benchmark.hpp>

namespace MLCMST::benchmark {

Benchmark::Benchmark(std::unique_ptr<Reporter> reporter) : _reporter(std::move(reporter))
{
}

Benchmark::~Benchmark() = default;

void Benchmark::addSolver(std::unique_ptr<MLCMSTSolver> solver, const std::string& name) {
    _solvers[name] = std::move(solver);
}

void Benchmark::addTestCase(const TestCase &test_case)
{
    _test_cases.push_back(test_case);
}

void Benchmark::run()
{
    std::unordered_map<std::string, std::vector<MLCMSTSolver::Result>> results;
    for (const auto& named_solver : _solvers) {
        std::vector<MLCMSTSolver::Result>& solver_results = results[named_solver.first];
        solver_results.clear();
        for (const TestCase& test_case : _test_cases) {
            MLCMSTSolver::Result result = named_solver.second->solve(test_case.mlccNetwork());
            solver_results.push_back(result);
        }
    }
    _reporter->report(_test_cases, results);
}

}
