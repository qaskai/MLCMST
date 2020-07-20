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

    static std::unordered_map<
            std::string, std::function<std::unique_ptr<MLCMSTSolver>(const Value& v)>
            > id_to_solver_builder;

    static std::pair<std::string, std::unique_ptr< MLCMSTSolver >> buildNamedSolver(const Value& v);
    static std::unique_ptr< MLCMSTSolver > buildSolver(const Value& v);
    static std::unique_ptr< MLCMSTSolver > buildLinkUpgradeUD(const Value& v);
    static std::unique_ptr< MLCMSTSolver > buildLocalSearch2006(const Value& v);
};
