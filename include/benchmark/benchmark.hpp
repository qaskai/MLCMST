#pragma once

#include <ostream>
#include <memory>

#include <benchmark/test_case.hpp>
#include <mlcmst_solver.hpp>

namespace MLCMST::benchmark {

class Benchmark
{
public:
    virtual ~Benchmark();

    virtual void addSolver(std::unique_ptr< MLCMSTSolver > solver) = 0;
    virtual void addTestCase(const TestCase& test_case) = 0;
    virtual void run() = 0;
    virtual void printReport(std::istream& out) = 0;

};

}
