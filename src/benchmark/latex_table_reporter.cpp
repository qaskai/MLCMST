#include <benchmark/latex_table_reporter.hpp>

#include <algorithm>

#include <util/util.hpp>

namespace MLCMST::benchmark {

LatexTableReporter::LatexTableReporter(std::ostream &out) : _out(out)
{
}

LatexTableReporter::~LatexTableReporter() = default;


void LatexTableReporter::report(const std::vector<TestCase> &test_cases,
                                const std::unordered_map<std::string, std::vector<MLCMST_Solver::Result>>& results)
{
    _out << "********** LatexTableReporter report **********\n";
    printLowerBoundGapTable(test_cases, results);
    printSolutionGapTable(test_cases, results);
    printTimeTable(results);
    _out << "\n";
}

void LatexTableReporter::printRow(const std::string& name, const std::vector<double> &v)
{
    if (v.empty()) {
        _out << "no stats...\n";
        return;
    }

    _out << "$" << util::mean(v) << "$ & ";
    _out << "$" << *std::min_element(v.begin(), v.end()) << " - " << *std::max_element(v.begin(), v.end()) << "$ & ";
    _out << "$" << util::stdev(v) << "$ & ";
    _out << "% " << name;
    _out << "\n";
}

void
LatexTableReporter::printTimeTable(const std::unordered_map<std::string, std::vector<MLCMST_Solver::Result>> &results)
{
    _out << "time table\n";
    _out << "avg, range, stdev\n";
    for (const auto& [name, solver_results] : results) {
        std::vector<double> solver_times;
        solver_times.reserve(solver_results.size());
        for (const auto& r : solver_results) {
            solver_times.push_back(r.wall_time);
        }

        printRow(name, solver_times);
    }
    _out << "--------------\n";
}

void
LatexTableReporter::printLowerBoundGapTable(const std::vector<TestCase> &test_cases,
                                    const std::unordered_map<std::string, std::vector<MLCMST_Solver::Result>> &results)
{
    _out << "lower bound gap table\n";
    _out << "avg, range, stdev\n";
    for (const auto& [name, solver_results] : results) {
        if (!solver_results[0].lower_bound.has_value()) {
            continue;
        }

        std::vector<double> solver_lower_bound_gaps;
        solver_lower_bound_gaps.reserve(solver_results.size());
        for (int i=0; i<solver_results.size(); i++) {
            double lower_bound = solver_results[i].lower_bound.value();
            double gap = (1.0 - (lower_bound/test_cases[i].lowerBound()));
            solver_lower_bound_gaps.push_back(gap * 100.0);
        }

        printRow(name, solver_lower_bound_gaps);
    }
    _out << "--------------\n";
}

void
LatexTableReporter::printSolutionGapTable(const std::vector<TestCase> &test_cases,
                                   const std::unordered_map<std::string, std::vector<MLCMST_Solver::Result>> &results)
{
    _out << "solution gap table\n";
    _out << "avg, range, stdev\n";
    for (const auto& [name, solver_results] : results) {
        if (!solver_results[0].mlcst.has_value()) {
            continue;
        }

        std::vector<double> solver_solution_gaps;
        solver_solution_gaps.reserve(solver_results.size());
        for (int i=0; i<solver_results.size(); i++) {
            const network::MLCST& mlcmst = solver_results[i].mlcst.value();
            double mlcmst_cost = mlcmst.cost(test_cases[i].mlccNetwork());
            double gap = (mlcmst_cost / test_cases[i].lowerBound() - 1.0);
            solver_solution_gaps.push_back(gap * 100.0);
        }

        printRow(name, solver_solution_gaps);
    }
    _out << "--------------\n";
}

}
