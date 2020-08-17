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

    virtual void report(const std::vector<TestCase>& test_cases,
                        const std::unordered_map<std::string, std::vector<MLCMST_Solver::Result>>& results) = 0;
};

}
