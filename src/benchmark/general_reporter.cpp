#include <benchmark/general_reporter.hpp>

#include <numeric>
#include <algorithm>

namespace MLCMST::benchmark {

GeneralReporter::~GeneralReporter() = default;

void GeneralReporter::printReport(
        std::vector<TestCase> &test_cases,
        std::unordered_map<std::string, std::vector<MLCMSTSolver::Result>> results,
        std::ostream &out)
{
    printTestCaseStats(test_cases);
    for (const auto& p : results) {
        printReport(p.first, test_cases, p.second, out);
    }
}

void GeneralReporter::printReport(const std::string& solver_name, std::vector<TestCase> &test_cases,
        const std::vector<MLCMSTSolver::Result>& results, std::ostream &out)
{
    out << "****************   " << solver_name << " statistics report" << "    **************\n";
    printLowerBoundGapStats(test_cases, results, out);
    printSolutionGapStats(test_cases, results, out);
    printTimeStats(results, out);
}

void GeneralReporter::printTestCaseStats(const std::vector<TestCase> &test_cases, std::ostream &out)
{
    out << "number of test cases: " << test_cases.size() << "\n";
    out << "graph order\n";
    std::vector<double> graph_orders;
    for (const TestCase& test_case : test_cases) {
        graph_orders.push_back(test_case.mlccNetwork().vertexCount());
    }
    printStatistics(graph_orders, out);
}

void GeneralReporter::printLowerBoundGapStats(const std::vector<TestCase> &test_cases,
        const std::vector<MLCMSTSolver::Result> &results, std::ostream &out)
{
    std::vector<double> lower_bound_gap;
    for (int i=0; i<test_cases.size(); i++) {
        if (!results[i].lower_bound.has_value())
            continue;
        double lower_bound = results[i].lower_bound.value();
        lower_bound_gap.push_back(1.0 - (lower_bound/test_cases[i].lowerBound()));
    }
    out << "lower bound gap of " << lower_bound_gap.size() << " qualified solutions\n";
    printStatistics(lower_bound_gap, out);
}

void GeneralReporter::printSolutionGapStats(const std::vector<TestCase> &test_cases,
        const std::vector<MLCMSTSolver::Result> &results, std::ostream &out)
{
    std::vector<double> lower_bound_gap;
    for (int i=0; i<test_cases.size(); i++) {
        if (!results[i].mlcmst.has_value())
            continue;
        double mlcmst_cost = results[i].mlcmst.value().cost(test_cases[i].mlccNetwork());
        lower_bound_gap.push_back((mlcmst_cost/test_cases[i].lowerBound() - 1.0));
    }
    out << "solution gap of " << lower_bound_gap.size() << " qualified solutions\n";
    printStatistics(lower_bound_gap, out);
}

void GeneralReporter::printTimeStats(const std::vector<MLCMSTSolver::Result> &results, std::ostream &out)
{
    std::vector<double> time_in_seconds;
    for (const MLCMSTSolver::Result result : results) {
        time_in_seconds.push_back(result.wall_time / 1000.0);
    }
    out << "runtime in seconds\n";
    printStatistics(time_in_seconds, out);
}

void GeneralReporter::printStatistics(const std::vector<double> &v, std::ostream &out)
{
    out << "avg " << average(v) << " ";
    out << "min " << *std::min_element(v.begin(), v.end()) << " ";
    out << "max " << *std::max_element(v.begin(), v.end()) << " ";
    out << "\n";
}

double GeneralReporter::average(const std::vector<double> &v)
{
    double sum = std::accumulate(v.begin(), v.end(), 0.0);
    return sum / v.size();
}


}
