#pragma once

#include <set>

#include <mlcmst_solver.hpp>
#include <heuristic/mlcmst_improver.hpp>
#include <mlcmst_subnet_solver.hpp>

#include <util/number/int_generator.hpp>

#include <json/property.hpp>

namespace MLCMST::heuristic {

class Martins2008_LocalSearch : public MLCMST_Improver
{
public:
    struct Params {
        int h_low, h_high;

        constexpr static auto properties = std::make_tuple(
            json::Property<Params, int>{&Params::h_low, "h_low"},
            json::Property<Params, int>{&Params::h_high, "h_high"}
            );
    };

    static std::string id();

    Martins2008_LocalSearch(std::unique_ptr< MLCMST_Solver > subnet_solver, Params params);
    Martins2008_LocalSearch(std::unique_ptr< MLCMST_Solver > subnet_solver, long seed, Params params);
    ~Martins2008_LocalSearch() override;

    network::MLCMST improve(network::MLCMST mlcmst, const network::MLCCNetwork &mlcc_network) override;

private:
    const network::MLCCNetwork* mlcc_network_;

    Params params;
    MLCMST_SubnetSolver subnet_solver_;
    util::number::IntGenerator int_generator_;

    void step(std::set<int>& S, network::MLCMST& mlcmst);
    std::pair<std::vector<int>, double> groupSubtrees(int i, const network::MLCMST& mlcmst);
    double subtreeCost(const std::vector<int>& vertices, const network::MLCMST& mlcmst);

    static std::set<int> vectorToSet(const std::vector<int>& v);
};

}
