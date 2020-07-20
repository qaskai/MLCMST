#include "solver_builder.hpp"

#include <stdexcept>

#include <mp/scf.hpp>
#include <mp/escf.hpp>
#include <mp/mcf.hpp>

#include <heuristic/link_upgrade_ud.hpp>
#include <heuristic/local_search_2006.hpp>
#include <heuristic/genetic_gamvros.hpp>

using namespace MLCMST;
using rapidjson::Value;

std::unordered_map<std::string, std::function<std::unique_ptr<MLCMSTSolver>(const Value& v)>>
SolverBuilder::id_to_solver_builder =
std::unordered_map<std::string, std::function<std::unique_ptr<MLCMSTSolver>(const Value& v)>>{
    std::make_pair(heuristic::LinkUpgradeUD::ID, SolverBuilder::buildLinkUpgradeUD),
    std::make_pair(heuristic::LocalSearch2006::ID, SolverBuilder::buildLocalSearch2006)
};

std::pair<std::string, std::unique_ptr<MLCMSTSolver >> SolverBuilder::buildNamedSolver(const Value &v)
{
    std::unique_ptr<MLCMSTSolver> solver = buildSolver(v);
    std::string name = v["id"].GetString();
    if (v.HasMember("name")) {
        name = v["name"].GetString();
    }
    return std::make_pair( name, std::move(solver) );
}

std::unique_ptr<MLCMSTSolver> SolverBuilder::buildSolver(const Value &v)
{
    if (!v.HasMember("id")) {
        throw std::invalid_argument("solver json must contain id field");
    }
    std::string id = v["id"].GetString();
    if (!id_to_solver_builder.count(id)) {
        throw std::invalid_argument("solver with id '" + id + "' does not exist");
    }
    return id_to_solver_builder[id](v);
}

std::unique_ptr<MLCMSTSolver> SolverBuilder::buildLinkUpgradeUD(const Value& v)
{
    return std::make_unique<heuristic::LinkUpgradeUD>();
}

std::unique_ptr<MLCMSTSolver> SolverBuilder::buildLocalSearch2006(const Value &v)
{
    const Value& params = v["params"];
    auto init_solver = buildSolver(params["init_solver"]);
    auto subproblem_solver = buildSolver(params["subproblem_solver"]);
    return std::make_unique<heuristic::LocalSearch2006>(
            std::move(init_solver), std::move(subproblem_solver));
}
