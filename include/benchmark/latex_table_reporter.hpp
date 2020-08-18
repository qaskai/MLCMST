#pragma once

#include <ostream>
#include <functional>

#include <benchmark/reporter.hpp>

namespace MLCMST::benchmark {

class LatexTableReporter : public Reporter
{
public:
    explicit LatexTableReporter(std::ostream& out);
    ~LatexTableReporter() override;

    void report(const std::vector<TestCase> &test_cases,
                const std::unordered_map<std::string, std::vector<MLCMST_Solver::Result>>& results) override;

private:
    static const double INF_;
    std::ostream& _out;

    void printLowerBoundGapTable(const std::vector<TestCase> &test_cases,
                                 const std::unordered_map<std::string, std::vector<MLCMST_Solver::Result>>& results);
    void printSolutionGapTable(const std::vector<TestCase> &test_cases,
                               const std::unordered_map<std::string, std::vector<MLCMST_Solver::Result>>& results);
    void printTimeTable(const std::unordered_map<std::string, std::vector<MLCMST_Solver::Result>>& results);

    void printTable(const std::function<double(const TestCase&, const MLCMST_Solver::Result&)>& getStat,
            const std::vector<TestCase> &test_cases,
            const std::unordered_map<std::string, std::vector<MLCMST_Solver::Result>>& results);
    void printRow(const std::string& name, const std::vector<double>& v);
};

}
