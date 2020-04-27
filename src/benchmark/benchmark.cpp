#include <benchmark/benchmark.hpp>

namespace MLCMST::benchmark {

Benchmark::Benchmark(std::unique_ptr<Reporter> reporter) : _reporter(std::move(reporter))
{
}

Benchmark::~Benchmark() = default;

void Benchmark::addSolver(std::unique_ptr<MLCMSTSolver> solver)
{
    // TODO: implementation
}

void Benchmark::addTestCase(const TestCase &test_case)
{
    // TODO: implementation
}

void Benchmark::run(std::ostream &out)
{
    // TODO: implementation
}

}
