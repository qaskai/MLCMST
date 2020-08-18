#include "solver_builder.hpp"

#include <stdexcept>

#include <mp/scf.hpp>
#include <mp/escf.hpp>
#include <mp/mcf.hpp>
#include <mp/capacity_indexed.hpp>

#include <heuristic/star.hpp>
#include <heuristic/esau_williams.hpp>
#include <heuristic/improvement/link_upgrade.hpp>
#include <heuristic/improvement/local_search_2006.hpp>
#include <heuristic/genetic_gamvros.hpp>
#include <heuristic/martins2008_construction.hpp>
#include <heuristic/improvement/martins2008_local_search.hpp>
#include <heuristic/grasp.hpp>
#include <heuristic/vns_campos.hpp>

#include <json/json.hpp>

using namespace MLCMST;
using rapidjson::Value;

const std::map<std::string, std::function<std::unique_ptr<MLCMST_Solver>(const Value& v)>>
SolverBuilder::id_to_solver_builder =
{
    { heuristic::Star::id(), buildStar },
    { heuristic::EsauWilliams::id(), buildEsauWilliams },
    { heuristic::improvement::LinkUpgrade::id(), SolverBuilder::buildLinkUpgrade },
    { heuristic::improvement::LocalSearch2006::id(), SolverBuilder::buildLocalSearch2006 },
    { heuristic::GeneticGamvros::id(), SolverBuilder::buildGeneticGamvros },
    { heuristic::GRASP::id(), SolverBuilder::buildGRASP },
    { heuristic::Martins2008_Construction::id(), SolverBuilder::buildMartins2008_Construction },
    { heuristic::improvement::Martins2008_LocalSearch::id(), SolverBuilder::buildMartins2008_LocalSearch },
    { heuristic::VNS_Campos::id(), SolverBuilder::buildVNS_Campos },
    { mp::SCF::id(), SolverBuilder::buildSCF },
    { mp::ESCF::id(), SolverBuilder::buildESCF },
    { mp::MCF::id(), SolverBuilder::buildMCF },
    { mp::CapacityIndexed::id(), SolverBuilder::buildCapacityIndexed }
};

std::pair<std::string, std::unique_ptr<MLCMST_Solver >> SolverBuilder::buildNamedSolver(const Value &v)
{
    std::unique_ptr<MLCMST_Solver> solver = buildSolver(v);
    std::string name = v["id"].GetString();
    if (v.HasMember("name")) {
        name = v["name"].GetString();
    }
    return std::make_pair( name, std::move(solver) );
}

std::unique_ptr<MLCMST_Solver> SolverBuilder::buildSolver(const Value &v)
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

std::unique_ptr<MLCMST_Solver> SolverBuilder::buildStar(const Value &v)
{
    return std::make_unique<heuristic::Star>();
}

std::unique_ptr<MLCMST_Solver> SolverBuilder::buildEsauWilliams(const Value &v)
{
    return std::make_unique<heuristic::EsauWilliams>();
}

std::unique_ptr<MLCMST_Solver> SolverBuilder::buildLinkUpgrade(const Value& v)
{
    const Value& params = v["params"];
    auto init_solver = buildSolver(params["init_solver"]);
    auto solver_params = json::fromJson< heuristic::improvement::LinkUpgrade::Params >(params);
    return std::make_unique<heuristic::improvement::LinkUpgrade>(solver_params);
}

std::unique_ptr<MLCMST_Solver> SolverBuilder::buildLocalSearch2006(const Value &v)
{
    const Value& params = v["params"];
    auto init_solver = buildSolver(params["init_solver"]);
    auto subproblem_solver = buildSolver(params["subnet_solver"]);
    auto solver_params = json::fromJson< heuristic::improvement::LocalSearch2006::Params >(params);
    return std::make_unique<heuristic::improvement::LocalSearch2006>(
            std::move(init_solver), std::move(subproblem_solver), solver_params);
}

std::unique_ptr<MLCMST_Solver> SolverBuilder::buildGeneticGamvros(const Value &v)
{
    const Value& params = v["params"];
    auto solver_params = json::fromJson<heuristic::GeneticGamvros::Params>(params);
    std::vector< std::unique_ptr< MLCMST_Solver > > init_solvers;
    std::transform(params["init_solvers"].Begin(), params["init_solvers"].End(),
            std::back_inserter(init_solvers), buildSolver);
    auto subnet_solver = buildSolver(params["subnet_solver"]);

    return std::make_unique<heuristic::GeneticGamvros>(
            std::move(init_solvers), std::move(subnet_solver), solver_params);
}

std::unique_ptr<MLCMST_Solver> SolverBuilder::buildGRASP(const Value &v)
{
    const Value& params = v["params"];
    std::unique_ptr<heuristic::MLCMST_Heuristic> construction(
            dynamic_cast<heuristic::MLCMST_Heuristic*>(buildSolver(params["construction_phase"]).release()));
    std::unique_ptr<heuristic::improvement::MLCMST_Improver> improvement(
            dynamic_cast<heuristic::improvement::MLCMST_Improver*>(buildSolver(params["improvement_phase"]).release()));
    auto solver_params = json::fromJson<heuristic::GRASP::Params>(params);
    return std::make_unique<heuristic::GRASP>(
        std::move(construction), std::move(improvement), solver_params);
}

std::unique_ptr<MLCMST_Solver> SolverBuilder::buildMartins2008_Construction(const Value &v)
{
    const Value& params = v["params"];
    auto solver_params = json::fromJson<heuristic::Martins2008_Construction::Params>(params);
    auto subnet_solver = buildSolver(params["subnet_solver"]);
    return std::make_unique<heuristic::Martins2008_Construction>(
            std::move(subnet_solver), solver_params
            );
}

std::unique_ptr<MLCMST_Solver> SolverBuilder::buildMartins2008_LocalSearch(const Value &v)
{
    const Value& params = v["params"];
    auto init_solver = buildSolver(params["init_solver"]);
    auto subnet_solver = buildSolver(params["subnet_solver"]);
    auto solver_params = json::fromJson<heuristic::improvement::Martins2008_LocalSearch::Params>(params);
    return std::make_unique<heuristic::improvement::Martins2008_LocalSearch>(
        std::move(init_solver), std::move(subnet_solver), solver_params);
}

std::unique_ptr<MLCMST_Solver> SolverBuilder::buildVNS_Campos(const Value &v)
{
    const Value& params = v["params"];
    std::unique_ptr<heuristic::MLCMST_Heuristic> init_solver(
            dynamic_cast<heuristic::MLCMST_Heuristic*>(buildSolver(params["init_solver"]).release()));
    auto solver_params = json::fromJson<heuristic::VNS_Campos::Params>(params);
    return std::make_unique<heuristic::VNS_Campos>(
            std::move(init_solver), solver_params);
}

std::unique_ptr<MLCMST_Solver> SolverBuilder::buildMPSolver(
        const Value &v, const std::function<std::unique_ptr<MLCMST_Solver>(bool)>& creator)
{
    const std::string id = v["id"].GetString();
    bool exact = v["params"]["exact"].GetBool();
    return creator(exact);
}

std::unique_ptr<MLCMST_Solver> SolverBuilder::buildSCF(const Value &v)
{
    return buildMPSolver(v, [](bool exact) { return std::make_unique<mp::SCF>(exact); });
}

std::unique_ptr<MLCMST_Solver> SolverBuilder::buildESCF(const Value &v)
{
    return buildMPSolver(v, [](bool exact) { return std::make_unique<mp::ESCF>(exact); });
}

std::unique_ptr<MLCMST_Solver> SolverBuilder::buildMCF(const Value &v)
{
    return buildMPSolver(v, [](bool exact) { return std::make_unique<mp::MCF>(exact); });
}

std::unique_ptr<MLCMST_Solver> SolverBuilder::buildCapacityIndexed(const Value &v)
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

const std::map<std::string, std::string> SolverBuilder::solver_json_template =
{

{ heuristic::Star::id(),
  "id == \"" + heuristic::Star::id() + "\", params:"
R""""(
{}
)"""" + "\n" },

{ heuristic::EsauWilliams::id(),
"id == \"" + heuristic::EsauWilliams::id() + "\", params:"
R""""(
{}
)"""" + "\n" },

{ heuristic::improvement::LinkUpgrade::id(),
  "id == \"" + heuristic::improvement::LinkUpgrade::id() + "\", params:"
R""""(
{
    "init_solver": <solver_json>,
    "H_leafs_only": bool,
    "check_all_link_types": bool,
    "reupgrade_nodes": bool
}
)"""" + "\n" },

{ heuristic::improvement::LocalSearch2006::id(),
  "id == \"" + heuristic::improvement::LocalSearch2006::id() + "\", params:"
R""""(
{
    "init_solver": <solver_json>,
    "subnet_solver": <solver_json>,
    "cycle_search_iterate_all": bool
}
)"""" + "\n"},

{ heuristic::GeneticGamvros::id(),
"id == \"" + heuristic::GeneticGamvros::id() + "\", params:"
R""""(
{
    "init_solvers": <solver_json> list,
    "subnet_solver": <solver_json>,
    "population_size": int,
    "most_fit_mutate_number": int,
    "parents_number": int,
    "generations_number": int,
    "network_fuzzing_epsilon": double,
    "sigma_truncation": double,
    "crossover_shrunk_move_probability": double,
    "crossover_move_less_than_k": int
}
)"""" + "\n"},

{ heuristic::GRASP::id(),
"id == \"" + heuristic::GRASP::id() + "\", params:"
R""""(
{
    "construction_phase": <solver_json> (of heuristic type),
    "improvement_phase": <solver_json> (of improver type),
    "iterations_no": int
}
)"""" + "\n"},

{ heuristic::Martins2008_Construction::id(),
"id == \"" + heuristic::Martins2008_Construction::id() + "\", params:"
R""""(
{
    "init_solver": <solver_json>,
    "subnet_solver": <solver_json>,
    "subnet_size": int,
    "alpha": double // in [0,1], 0-deterministic, 1-completely random
}
)"""" + "\n"},

{ heuristic::improvement::Martins2008_LocalSearch::id(),
"id == \"" + heuristic::improvement::Martins2008_LocalSearch::id() + "\", params:"
R""""(
{
    "init_solver": <solver_json>,
    "subnet_solver": <solver_json>,
    "h_low": int,
    "h_high": int
}
)"""" + "\n"},


{ heuristic::VNS_Campos::id(),
"id == \"" + heuristic::VNS_Campos::id() + "\", params:"
R""""(
{
    "init_solver": <solver_json>,
    "max_failed_iterations": int
}
)"""" + "\n"},

{ mp::SCF::id(),
  "id == \"" + mp::SCF::id() + "\", params:"
R""""(
{ "exact": bool }
)"""" + "\n"},

{ mp::ESCF::id(),
  "id == \"" + mp::ESCF::id() + "\", params:"
R""""(
{ "exact": bool }
)"""" + "\n"},

{ mp::MCF::id(),
  "id == \"" + mp::MCF::id() + "\", params:"
R""""(
{ "exact": bool }
)"""" + "\n"},

{ mp::CapacityIndexed::id(),
  "id == \"" + mp::CapacityIndexed::id() + "\", params:"
R""""(
{ "exact": bool }
)"""" + "\n"}

};

