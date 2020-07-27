#pragma once

#include <unordered_map>
#include <functional>
#include <memory>

#include <rapidjson/document.h>

#include <mlcmst_solver.hpp>

class SolverBuilder
{
public:
    using MLCMSTSolver = MLCMST::MLCMSTSolver;
    using Value = rapidjson::Value;

    static const std::unordered_map<
            std::string, std::function<std::unique_ptr<MLCMSTSolver>(const Value& v)>
            > id_to_solver_builder;

    static std::pair<std::string, std::unique_ptr< MLCMSTSolver >> buildNamedSolver(const Value& v);
    static const std::unordered_map<std::string, std::string> solver_json_template;

private:
    static std::unique_ptr< MLCMSTSolver > buildSolver(const Value& v);
    static std::unique_ptr< MLCMSTSolver > buildLinkUpgrade(const Value& v);
    static std::unique_ptr< MLCMSTSolver > buildLocalSearch2006(const Value& v);
    static std::unique_ptr< MLCMSTSolver > buildGeneticGamvros(const Value& v);
    static std::unique_ptr< MLCMSTSolver > buildSCF(const Value& v);
    static std::unique_ptr< MLCMSTSolver > buildESCF(const Value& v);
    static std::unique_ptr< MLCMSTSolver > buildMCF(const Value& v);
    static std::unique_ptr< MLCMSTSolver > buildCapacityIndexed(const Value& v);

    static std::unique_ptr< MLCMSTSolver > buildMPSolver(
            const Value& v, const std::function<std::unique_ptr<MLCMSTSolver>(bool)>& creator);

    static bool checkContainsMembers(const Value& v, const std::vector<std::string>& required_member_ids);
};
