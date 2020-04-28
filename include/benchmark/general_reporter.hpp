#pragma once

#include <benchmark/reporter.hpp>

namespace MLCMST::benchmark {

class GeneralReporter : public Reporter
{
public:
    ~GeneralReporter() override;

    void printReport(std::vector<TestCase>& test_cases,
            std::unordered_map<std::string, std::vector<MLCMSTSolver::Result>> results, std::ostream& out) override;

private:
    void printReport(const std::string& solver_name, std::vector<TestCase>& test_cases,
            const std::vector<MLCMSTSolver::Result>& results, std::ostream& out);

    void printTestCaseStats(const std::vector<TestCase>& test_cases, std::ostream& out);
    void printLowerBoundGapStats(const std::vector<TestCase>& test_cases,
            const std::vector<MLCMSTSolver::Result>& results, std::ostream& out);
    void printSolutionGapStats(const std::vector<TestCase>& test_cases,
            const std::vector<MLCMSTSolver::Result>& results, std::ostream& out);
    void printTimeStats(const std::vector<MLCMSTSolver::Result>& results, std::ostream& out);

    void printStatistics(const std::vector<double>& v, std::ostream& out);
    double average(const std::vector<double>& v);

};

}
