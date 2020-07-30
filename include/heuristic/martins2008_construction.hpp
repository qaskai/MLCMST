#pragma once

#include <set>
#include <string>

#include <network/mlcc_network.hpp>

#include <mlcmst_solver.hpp>
#include <mlcmst_subnet_solver.hpp>
#include <heuristic/mlcmst_heuristic.hpp>

#include <util/number/int_generator.hpp>

#include <json/property.hpp>

namespace MLCMST::heuristic {

class Martins2008_Construction : public MLCMST_Heuristic
{
public:
    struct Params {
        int subnet_size;
        double alpha; // in [0,1], 0-deterministic, 1-completely random

        constexpr static auto properties = std::make_tuple(
            json::Property<Params, int>{&Params::subnet_size, "subnet_size"},
            json::Property<Params, double>{&Params::alpha, "alpha"}
            );
    };

    static std::string id();

    explicit Martins2008_Construction(std::unique_ptr< MLCMST_Solver > subnet_solver, Params params);
    Martins2008_Construction(std::unique_ptr< MLCMST_Solver > subnet_solver, long seed, Params params);
    ~Martins2008_Construction() override;

    network::MLCMST run(const network::MLCCNetwork &mlcc_network) override;

private:
    const network::MLCCNetwork* mlcc_network_;

    Params params;
    MLCMST_SubnetSolver subnet_solver_;
    util::number::IntGenerator int_generator_;

    std::vector<int> createNewGroup(const std::set<int>& unassigned);

};


}
