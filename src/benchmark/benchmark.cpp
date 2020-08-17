#include <benchmark/benchmark.hpp>

#include <iostream>

namespace MLCMST::benchmark {

Benchmark::Benchmark(std::vector<std::unique_ptr<Reporter>> reporters) : _reporters(std::move(reporters))
{
}

Benchmark::Benchmark(Benchmark &&benchmark) noexcept : _reporters(std::move(benchmark._reporters)),
    _test_cases(std::move(benchmark._test_cases)), _solvers(std::move(benchmark._solvers))
{
}

Benchmark::~Benchmark() = default;

void Benchmark::addSolver(std::unique_ptr<MLCMST_Solver> solver, const std::string& name) {
    _solvers[name] = std::move(solver);
}

void Benchmark::addTestCase(const TestCase &test_case)
{
    _test_cases.push_back(test_case);
}

void Benchmark::run()
{
    std::unordered_map<std::string, std::vector<MLCMST_Solver::Result>> results;
    for (const auto& named_solver : _solvers) {
        std::vector<MLCMST_Solver::Result>& solver_results = results[named_solver.first];
        solver_results.clear();
        for (const TestCase &test_case : _test_cases) {
            MLCMST_Solver::Result result = named_solver.second->solve(test_case.mlccNetwork());
            solver_results.push_back(result);
            std::cerr << "." << std::flush;
        }
    }
    std::cerr << std::endl;
    for (const auto& reporter : _reporters) {
        reporter->report(_test_cases, results);
    }
}

}
