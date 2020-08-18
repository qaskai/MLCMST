#include <benchmark/latex_table_reporter.hpp>

#include <algorithm>
#include <iomanip>

#include <util/util.hpp>

namespace MLCMST::benchmark {

const double LatexTableReporter::INF_ = std::numeric_limits<double>::infinity();

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

    // save stream settings
    std::ios state(nullptr);
    state.copyfmt(_out);

    _out << std::setprecision(2) << std::fixed;
    _out << "$" << util::mean(v) << "$ & ";
    _out << "$" << *std::min_element(v.begin(), v.end()) << " - " << *std::max_element(v.begin(), v.end()) << "$ & ";
    _out << "$" << util::stdev(v) << "$ & ";
    _out << "% " << name;
    _out << "\n";

    // restore stream settings
    _out.copyfmt(state);
}

void LatexTableReporter::printTable(
        const std::function<double(const TestCase &, const MLCMST_Solver::Result &)>& getStat,
        const std::vector <TestCase> &test_cases,
        const std::unordered_map<std::string, std::vector<MLCMST_Solver::Result>> &results)
{
    _out << "avg, range, stdev\n";
    for (const auto& [name, solver_results] : results) {
        std::vector<double> stats;
        stats.reserve(solver_results.size());
        for (int i=0; i<solver_results.size(); i++) {
            stats.push_back(getStat(test_cases[i], solver_results[i]));
        }
        if (std::find(stats.begin(), stats.end(), INF_) == stats.end()) {
            printRow(name, stats);
        }
    }
}

void
LatexTableReporter::printTimeTable(const std::unordered_map<std::string, std::vector<MLCMST_Solver::Result>> &results)
{
    _out << "time table (in seconds)\n";
    auto getStat = [] (const TestCase& test_case, const MLCMST_Solver::Result& result) {
        return result.wall_time / 1000.0;
    };
    printTable(getStat, {}, results);
    _out << "--------------\n";
}

void
LatexTableReporter::printLowerBoundGapTable(const std::vector<TestCase> &test_cases,
                                    const std::unordered_map<std::string, std::vector<MLCMST_Solver::Result>> &results)
{
    _out << "lower bound gap table (in %)\n";

    auto getStat = [] (const TestCase& test_case, const MLCMST_Solver::Result& result) {
        if (!result.lower_bound.has_value()) {
            return INF_;
        }
        double lower_bound = result.lower_bound.value();
        double gap = (1.0 - (lower_bound/test_case.lowerBound()));
        return gap * 100.0;
    };
    printTable(getStat, test_cases, results);
    _out << "--------------\n";
}

void
LatexTableReporter::printSolutionGapTable(const std::vector<TestCase> &test_cases,
                                   const std::unordered_map<std::string, std::vector<MLCMST_Solver::Result>> &results)
{
    _out << "solution gap table (in %)\n";

    auto getStat = [] (const TestCase& test_case, const MLCMST_Solver::Result& result) {
        if (!result.mlcst.has_value()) {
            return INF_;
        }
        const network::MLCST& mlcmst = result.mlcst.value();
        double mlcmst_cost = mlcmst.cost(test_case.mlccNetwork());
        double gap = (mlcmst_cost / test_case.lowerBound() - 1.0);
        return gap * 100.0;
    };
    printTable(getStat, test_cases, results);
    _out << "--------------\n";
}

}
