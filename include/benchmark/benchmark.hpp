#pragma once

#include <ostream>
#include <memory>

#include <benchmark/test_case.hpp>
#include <benchmark/reporter.hpp>
#include <mlcmst_solver.hpp>

namespace MLCMST::benchmark {

class Benchmark
{
public:
    explicit Benchmark(std::unique_ptr< Reporter > reporter);
    ~Benchmark();

    void addSolver(std::unique_ptr< MLCMSTSolver > solver);
    void addTestCase(const TestCase& test_case);
    void run(std::ostream& out);

private:
    std::unique_ptr< Reporter > _reporter;
};

}
