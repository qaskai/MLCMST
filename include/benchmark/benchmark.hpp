#pragma once

#include <string>
#include <ostream>
#include <memory>
#include <vector>
#include <unordered_map>

#include <benchmark/test_case.hpp>
#include <benchmark/reporter.hpp>
#include <mlcmst_solver.hpp>

namespace MLCMST::benchmark {

class Benchmark
{
public:
    explicit Benchmark(std::unique_ptr< Reporter > reporter);
    Benchmark(Benchmark&& benchmark) noexcept;
    ~Benchmark();

    void addSolver(std::unique_ptr< MLCMST_Solver > solver, const std::string& name);
    void addTestCase(const TestCase& test_case);
    void run();

private:
    std::unique_ptr< Reporter > _reporter;
    std::vector<TestCase> _test_cases;
    std::unordered_map<std::string, std::unique_ptr< MLCMST_Solver >> _solvers;
};

}
