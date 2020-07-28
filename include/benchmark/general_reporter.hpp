#pragma once

#include <benchmark/reporter.hpp>

namespace MLCMST::benchmark {

class GeneralReporter : public Reporter
{
public:
    explicit GeneralReporter(std::ostream& out);
    ~GeneralReporter() override;

    void report(std::vector<TestCase>& test_cases,
                std::unordered_map<std::string, std::vector<MLCMST_Solver::Result>> results) override;

private:
    std::ostream& _out;

    void printReport(const std::string& solver_name, std::vector<TestCase>& test_cases,
            const std::vector<MLCMST_Solver::Result>& results);

    void printTestCaseStats(const std::vector<TestCase>& test_cases);
    void printLowerBoundGapStats(const std::vector<TestCase>& test_cases, const std::vector<MLCMST_Solver::Result>& results);
    void printSolutionGapStats(const std::vector<TestCase>& test_cases, const std::vector<MLCMST_Solver::Result>& results);
    void printTimeStats(const std::vector<MLCMST_Solver::Result>& results);

    void printStatistics(const std::vector<double>& v);

};

}
