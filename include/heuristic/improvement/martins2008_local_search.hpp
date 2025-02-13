#pragma once

#include <set>

#include <mlcmst_solver.hpp>
#include <heuristic/improvement/mlcmst_improver.hpp>
#include <mlcmst_subnet_solver.hpp>

#include <util/number/int_generator.hpp>

#include <json/property.hpp>

namespace MLCMST::heuristic::improvement {

class Martins2008_LocalSearch : public improvement::MLCMST_Improver
{
public:
    struct Params {
        int subnet_size_low, subnet_size_high;

        constexpr static auto properties = std::make_tuple(
            json::Property<Params, int>{&Params::subnet_size_low, "subnet_size_low"},
            json::Property<Params, int>{&Params::subnet_size_high, "subnet_size_high"}
            );
    };

    static std::string id();

    Martins2008_LocalSearch(std::unique_ptr< MLCMST_Solver > init_solver,
            std::unique_ptr< MLCMST_Solver > subnet_solver,Params params);
    Martins2008_LocalSearch(std::unique_ptr< MLCMST_Solver > init_solver,
            std::unique_ptr< MLCMST_Solver > subnet_solver, long seed, Params params);
    ~Martins2008_LocalSearch() override;

    network::MLCST improve(long steps, network::MLCST mlcmst, const network::MLCCNetwork &mlcc_network) override;

private:
    const network::MLCCNetwork* mlcc_network_ = nullptr;

    Params params;
    MLCMST_SubnetSolver subnet_solver_;
    util::number::IntGenerator int_generator_;

    void step(std::set<int>& S, network::MLCST& mlcmst);
    std::pair<std::vector<int>, double> groupSubtrees(int i, const network::MLCST& mlcmst);
    double subtreeCost(const std::vector<int>& vertices, const network::MLCST& mlcmst);

    static std::set<int> vectorToSet(const std::vector<int>& v);
};

}
