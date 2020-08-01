#pragma once

#include <string>

#include <heuristic/mlcmst_heuristic.hpp>

#include <network/mlcc_network.hpp>
#include <network/capacitated_network.hpp>

namespace MLCMST::heuristic {

class EsauWilliams : public MLCMST_Heuristic
{
public:
    static std::string id();

    ~EsauWilliams() override;

    network::MLCMST run(const network::MLCCNetwork &mlcc_network) override;

private:
    static std::vector<int> run(int root, const network::CapacitatedNetwork& network, const std::vector<int>& demands);

    static std::vector<int> directedTreeFromEdges(int N, int root, const std::vector<std::pair<int,int>>& edges);

};

}
