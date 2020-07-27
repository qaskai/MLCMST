#include "solver_builder.hpp"

#include <stdexcept>

#include <mp/scf.hpp>
#include <mp/escf.hpp>
#include <mp/mcf.hpp>
#include <mp/capacity_indexed.hpp>

#include <heuristic/link_upgrade.hpp>
#include <heuristic/local_search_2006.hpp>
#include <heuristic/genetic_gamvros.hpp>

using namespace MLCMST;
using rapidjson::Value;

const std::unordered_map<std::string, std::function<std::unique_ptr<MLCMSTSolver>(const Value& v)>>
SolverBuilder::id_to_solver_builder =
{
    { heuristic::LinkUpgrade::id(), SolverBuilder::buildLinkUpgrade },
    { heuristic::LocalSearch2006::id(), SolverBuilder::buildLocalSearch2006 },
    { heuristic::GeneticGamvros::id(), SolverBuilder::buildGeneticGamvros },
    { mp::SCF::id(), SolverBuilder::buildSCF },
    { mp::ESCF::id(), SolverBuilder::buildESCF },
    { mp::MCF::id(), SolverBuilder::buildMCF },
    { mp::CapacityIndexed::id(), SolverBuilder::buildCapacityIndexed }
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
    return id_to_solver_builder.at(id)(v);
}

std::unique_ptr<MLCMSTSolver> SolverBuilder::buildLinkUpgrade(const Value& v)
{
    const std::string id = heuristic::LinkUpgrade::id();
    if (!v.HasMember("params"))
        throw std::invalid_argument(solver_json_template.at(id));
    const Value& params = v["params"];
    const std::vector<std::string> required_params{
        "H_leafs_only", "check_all_link_types", "reupgrade_nodes"
    };
    if (!checkContainsMembers(params, required_params)) {
        throw std::invalid_argument(solver_json_template.at(id));
    }
    heuristic::LinkUpgrade::Params solver_params{
        .H_leafs_only= params["H_leafs_only"].GetBool(),
        .check_all_link_types= params["check_all_link_types"].GetBool(),
        .reupgrade_nodes= params["reupgrade_nodes"].GetBool()
    };

    return std::make_unique<heuristic::LinkUpgrade>(solver_params);
}

std::unique_ptr<MLCMSTSolver> SolverBuilder::buildLocalSearch2006(const Value &v)
{
    const std::string id = heuristic::LocalSearch2006::id();
    if (!v.HasMember("params"))
        throw std::invalid_argument(solver_json_template.at(id));
    const Value& params = v["params"];
    if (!params.HasMember("init_solver") || !params.HasMember("subnet_solver"))
        throw std::invalid_argument(solver_json_template.at(id));

    auto init_solver = buildSolver(params["init_solver"]);
    auto subproblem_solver = buildSolver(params["subnet_solver"]);
    return std::make_unique<heuristic::LocalSearch2006>(
            std::move(init_solver), std::move(subproblem_solver));
}

std::unique_ptr<MLCMSTSolver> SolverBuilder::buildGeneticGamvros(const Value &v)
{
    std::string id = heuristic::GeneticGamvros::id();
    if (!v.HasMember("params"))
        throw std::invalid_argument(solver_json_template.at(id));
    const Value& params = v["params"];
    const std::vector<std::string> required_params{
        "init_solvers", "subnet_solver",
        "population_size", "most_fit_mutate_number", "parents_number", "generations_number",
        "network_fuzzing_epsilon", "crossover_shrunk_move_probability", "crossover_move_less_than_k"
    };
    if (!checkContainsMembers(params, required_params)) {
        throw std::invalid_argument(solver_json_template.at(id));
    }

    heuristic::GeneticGamvros::Params solver_params{
        .population_size = params["population_size"].GetInt(),
        .most_fit_mutate_number = params["most_fit_mutate_number"].GetInt(),
        .parents_number = params["parents_number"].GetInt(),
        .generations_number = params["generations_number"].GetInt(),
        .network_fuzzing_epsilon = params["network_fuzzing_epsilon"].GetDouble(),
        .crossover_shrunk_move_probability = params["crossover_shrunk_move_probability"].GetDouble(),
        .crossover_move_less_than_k = params["crossover_move_less_than_k"].GetInt()
    };
    std::vector< std::unique_ptr< MLCMSTSolver > > init_solvers;
    std::transform(params["init_solvers"].Begin(), params["init_solvers"].End(), std::back_inserter(init_solvers),
        [] (const Value& solver) {
            return buildSolver(solver);
        });
    auto subnet_solver = buildSolver(params["subnet_solver"]);

    return std::make_unique<heuristic::GeneticGamvros>(
            std::move(init_solvers), std::move(subnet_solver), solver_params);
}

std::unique_ptr<MLCMSTSolver> SolverBuilder::buildMPSolver(
        const Value &v, const std::function<std::unique_ptr<MLCMSTSolver>(bool)>& creator)
{
    const std::string id = v["id"].GetString();

    bool exact = false;
    if (!v.HasMember("params") || !v["params"].HasMember("exact")) {
        throw std::invalid_argument(solver_json_template.at(id));
    }
    exact = v["params"]["exact"].GetBool();
    return creator(exact);
}

std::unique_ptr<MLCMSTSolver> SolverBuilder::buildSCF(const Value &v)
{
    return buildMPSolver(v, [](bool exact) { return std::make_unique<mp::SCF>(exact); });
}

std::unique_ptr<MLCMSTSolver> SolverBuilder::buildESCF(const Value &v)
{
    return buildMPSolver(v, [](bool exact) { return std::make_unique<mp::ESCF>(exact); });
}

std::unique_ptr<MLCMSTSolver> SolverBuilder::buildMCF(const Value &v)
{
    return buildMPSolver(v, [](bool exact) { return std::make_unique<mp::MCF>(exact); });
}

std::unique_ptr<MLCMSTSolver> SolverBuilder::buildCapacityIndexed(const Value &v)
{
    return buildMPSolver(v, [](bool exact) { return std::make_unique<mp::CapacityIndexed>(exact); });
}

bool SolverBuilder::checkContainsMembers(const Value& v, const std::vector<std::string>& required_member_ids)
{
    for (const std::string& s : required_member_ids) {
        if (!v.HasMember(rapidjson::StringRef(s.c_str())))
            return false;
    }
    return true;
}

const std::unordered_map<std::string, std::string> SolverBuilder::solver_json_template =
{

{ heuristic::LinkUpgrade::id(),
  "id == " + heuristic::LinkUpgrade::id() + " params:"
R""""(
{
    "H_leafs_only": bool,
    "check_all_link_types": bool, // if true will iterate over viable link upgrades to find best H, if not L,L-1,...,0
    "reupgrade_nodes": bool
}
)"""" + "\n" },

{ heuristic::LocalSearch2006::id(),
  "id == " + heuristic::LocalSearch2006::id() + " params:"
R""""(
{
    "init_solver": <solver_json>,
    "subnet_solver": <solver_json>
}
)"""" + "\n"},

{ heuristic::GeneticGamvros::id(),
  "id == " + heuristic::GeneticGamvros::id() + " params:"
R""""(
{
    "init_solvers": <solver_json> list,
    "subnet_solver": <solver_json>,
    "population_size": int,
    "most_fit_mutate_number": int,
    "parents_number": int,
    "generations_number": int,
    "network_fuzzing_epsilon": double,
    "crossover_shrunk_move_probability": double,
    "crossover_move_less_than_k": int
}
)"""" + "\n"},

{ mp::SCF::id(),
  "id == " + mp::SCF::id() + " params:"
R""""(
{ "exact": bool }
)"""" + "\n"},

{ mp::ESCF::id(),
  "id == " + mp::ESCF::id() + " params:"
R""""(
{ "exact": bool }
)"""" + "\n"},

{ mp::MCF::id(),
  "id == " + mp::MCF::id() + " params:"
R""""(
{ "exact": bool }
)"""" + "\n"},

{ mp::CapacityIndexed::id(),
  "id == " + mp::MCF::id() + " params:"
R""""(
{ "exact": bool }
)"""" + "\n"},

};

