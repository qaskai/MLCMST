#pragma once

#include <memory>
#include <string>
#include <tuple>

#include <heuristic/mlcmst_heuristic.hpp>
#include <heuristic/improvement/mlcmst_improver.hpp>

#include <json/property.hpp>

namespace MLCMST::heuristic {

class GRASP : public MLCMST_Heuristic
{
public:
    static std::string id();

    struct Params
    {
        int iterations_no;

        static constexpr auto properties = std::make_tuple(
            json::Property<Params, int>{ &Params::iterations_no, "iterations_no" }
            );
    };

    GRASP(std::unique_ptr< MLCMST_Heuristic > construction_phase,
        std::unique_ptr< improvement::MLCMST_Improver > improvement_phase,
        Params params);
    ~GRASP() override;

    network::MLCMST run(const network::MLCCNetwork &mlcc_network) override;

private:
    Params params;
    std::unique_ptr< MLCMST_Heuristic > construction_phase_;
    std::unique_ptr< improvement::MLCMST_Improver > improvement_phase_;
};

}
