#include <benchmark/general_reporter.hpp>

#include <numeric>
#include <algorithm>

namespace MLCMST::benchmark {

GeneralReporter::GeneralReporter(std::ostream &out) : _out(out)
{
}

GeneralReporter::~GeneralReporter() = default;

void GeneralReporter::report(
        std::vector<TestCase> &test_cases,
        std::unordered_map<std::string, std::vector<MLCMSTSolver::Result>> results)
{
    printTestCaseStats(test_cases);
    for (const auto& p : results) {
        printReport(p.first, test_cases, p.second);
    }
}

void GeneralReporter::printReport(const std::string& solver_name, std::vector<TestCase> &test_cases,
        const std::vector<MLCMSTSolver::Result>& results)
{
    _out << "****************   " << solver_name << " statistics report" << "    **************\n";
    printLowerBoundGapStats(test_cases, results);
    printSolutionGapStats(test_cases, results);
    printTimeStats(results);
}

void GeneralReporter::printTestCaseStats(const std::vector<TestCase> &test_cases)
{
    _out << "number of test cases: " << test_cases.size() << "\n";
    _out << "graph order\n";
    std::vector<double> graph_orders;
    for (const TestCase &test_case : test_cases) {
        graph_orders.push_back(test_case.mlccNetwork().vertexCount());
    }
    printStatistics(graph_orders);
}

void GeneralReporter::printLowerBoundGapStats(const std::vector<TestCase> &test_cases,
        const std::vector<MLCMSTSolver::Result> &results)
{
    std::vector<double> lower_bound_gap;
    for (unsigned i=0; i<test_cases.size(); i++) {
        if (!results[i].lower_bound.has_value())
            continue;
        double lower_bound = results[i].lower_bound.value();
        lower_bound_gap.push_back(1.0 - (lower_bound/test_cases[i].lowerBound()));
    }
    _out << "lower bound gap of " << lower_bound_gap.size() << " qualified solutions\n";
    printStatistics(lower_bound_gap);
}

void GeneralReporter::printSolutionGapStats(const std::vector<TestCase> &test_cases,
        const std::vector<MLCMSTSolver::Result> &results)
{
    std::vector<double> lower_bound_gap;
    for (unsigned i=0; i<test_cases.size(); i++) {
        if (!results[i].mlcmst.has_value())
            continue;
        double mlcmst_cost = results[i].mlcmst.value().cost(test_cases[i].mlccNetwork());
        lower_bound_gap.push_back((mlcmst_cost/test_cases[i].lowerBound() - 1.0));
    }
    _out << "solution gap of " << lower_bound_gap.size() << " qualified solutions\n";
    printStatistics(lower_bound_gap);
}

void GeneralReporter::printTimeStats(const std::vector<MLCMSTSolver::Result> &results)
{
    std::vector<double> time_in_seconds;
    for (const MLCMSTSolver::Result& result : results) {
        time_in_seconds.push_back(result.wall_time / 1000.0);
    }
    _out << "runtime in seconds\n";
    printStatistics(time_in_seconds);
}

void GeneralReporter::printStatistics(const std::vector<double> &v)
{
    if (v.empty()) {
        _out << "no stats...\n";
        return;
    }
    _out << "avg " << average(v) << " ";
    _out << "min " << *std::min_element(v.begin(), v.end()) << " ";
    _out << "max " << *std::max_element(v.begin(), v.end()) << " ";
    _out << "\n";
}

double GeneralReporter::average(const std::vector<double> &v)
{
    double sum = std::accumulate(v.begin(), v.end(), 0.0);
    return sum / v.size();
}


}
