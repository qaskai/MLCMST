#pragma once

#include <ostream>
#include <vector>
#include <unordered_map>
#include <string>

#include <benchmark/test_case.hpp>
#include <mlcmst_solver.hpp>

namespace MLCMST::benchmark {

class Reporter
{
public:
    virtual ~Reporter();

    virtual void printReport(
        std::vector<TestCase>& test_cases,
        std::unordered_map<std::string, std::vector<MLCMSTSolver::Result>> results,
        std::ostream& out) = 0;
};

}
