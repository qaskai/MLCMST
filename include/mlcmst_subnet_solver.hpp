#pragma once

#include <memory>
#include <vector>
#include <unordered_map>

#include <mlcmst_solver.hpp>
#include <network/mlcst.hpp>
#include <network/mlcc_network.hpp>

namespace MLCMST {

class MLCMST_SubnetSolver final
{
public:
    struct Result {
        double cost;
        network::MLCST mlcmst;
        std::vector<int> mapping;
    };

    explicit MLCMST_SubnetSolver(std::unique_ptr<MLCMST_Solver> solver);
    ~MLCMST_SubnetSolver();

    std::pair<network::MLCST, std::vector<int>> subnetTree(
            const network::MLCCNetwork& network, const std::vector<int>& subnet_vertices);
    double subnetTreeCost(const network::MLCCNetwork& network, const std::vector<int>& subnet_vertices);

    std::unordered_map<int, std::pair< network::MLCST, std::vector<int> >> allSubnetTrees(
            const network::MLCCNetwork& network, const std::vector<int>& vertex_subnet);
    std::unordered_map<int, double> allSubnetTreeCosts(
            const network::MLCCNetwork& network, const std::vector<int>& vertex_subnet);

    std::unordered_map<int, std::pair< network::MLCST, std::vector<int> > >allSubnetTrees(
            const network::MLCCNetwork& network, const std::unordered_map<int,std::vector<int>>& groups);
    std::unordered_map<int, double> allSubnetTreeCosts(
            const network::MLCCNetwork& network, const std::unordered_map<int,std::vector<int>>& groups);

    std::unordered_map<int, Result> solveAllSubnets(
            const network::MLCCNetwork& network, const std::vector<int>& subnet_vertices);
    std::unordered_map<int, Result> solveAllSubnets(
            const network::MLCCNetwork& network, const std::unordered_map<int, std::vector<int>>& groups);

    Result solveSubnet(const network::MLCCNetwork& network, std::vector<int> subnet_vertices);

    network::MLCST solveMLCMST(const network::MLCCNetwork& network, const std::vector<int>& vertex_subnet);
    network::MLCST solveMLCMST(
            const network::MLCCNetwork& network, const std::unordered_map<int, std::vector<int>>& groups);



private:
    std::unique_ptr< MLCMST_Solver > solver_;

    std::unordered_map<int, std::vector<int>> createGroups(int center, const std::vector<int>& vertex_subnet);
};

}
