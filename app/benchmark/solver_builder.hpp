#pragma once

#include <map>
#include <functional>
#include <memory>

#include <rapidjson/document.h>

#include <mlcmst_solver.hpp>

class SolverBuilder
{
public:
    using MLCMST_Solver = MLCMST::MLCMST_Solver;
    using Value = rapidjson::Value;

    static const std::map<
            std::string, std::function<std::unique_ptr<MLCMST_Solver>(const Value& v)>
            > id_to_solver_builder;

    static std::pair<std::string, std::unique_ptr< MLCMST_Solver >> buildNamedSolver(const Value& v);
    static const std::map<std::string, std::string> solver_json_template;

private:
    // mlcmst solver
    static std::unique_ptr< MLCMST_Solver > buildSolver(const Value& v);

    static std::unique_ptr< MLCMST_Solver > buildStar(const Value& v);
    static std::unique_ptr< MLCMST_Solver > buildEsauWilliams(const Value& v);
    static std::unique_ptr< MLCMST_Solver > buildLinkUpgrade(const Value& v);
    static std::unique_ptr< MLCMST_Solver > buildLocalSearch2006(const Value& v);
    static std::unique_ptr< MLCMST_Solver > buildGeneticGamvros(const Value& v);
    static std::unique_ptr< MLCMST_Solver > buildMartins2008_Construction(const Value& v);
    static std::unique_ptr< MLCMST_Solver > buildMartins2008_LocalSearch(const Value& v);

    // mlcmst improver

    // mp
    static std::unique_ptr< MLCMST_Solver > buildSCF(const Value& v);
    static std::unique_ptr< MLCMST_Solver > buildESCF(const Value& v);
    static std::unique_ptr< MLCMST_Solver > buildMCF(const Value& v);
    static std::unique_ptr< MLCMST_Solver > buildCapacityIndexed(const Value& v);

    static std::unique_ptr< MLCMST_Solver > buildMPSolver(
            const Value& v, const std::function<std::unique_ptr<MLCMST_Solver>(bool)>& creator);

    static bool checkContainsMembers(const Value& v, const std::vector<std::string>& required_member_ids);
};
