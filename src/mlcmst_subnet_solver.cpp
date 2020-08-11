#include <mlcmst_subnet_solver.hpp>

#include <algorithm>

namespace MLCMST {

MLCMST_SubnetSolver::MLCMST_SubnetSolver(std::unique_ptr<MLCMST_Solver> solver) : solver_(std::move(solver))
{
}

MLCMST_SubnetSolver::~MLCMST_SubnetSolver() = default;

std::pair<network::MLCST, std::vector<int>>
MLCMST_SubnetSolver::subnetTree(const network::MLCCNetwork &network, const std::vector<int>& subnet_vertices)
{
    auto result = solveSubnet(network, subnet_vertices);
    return std::make_pair(result.mlcmst, result.mapping);
}

double MLCMST_SubnetSolver::subnetTreeCost(const network::MLCCNetwork &network, const std::vector<int>& subnet_vertices)
{
    return solveSubnet(network, subnet_vertices).cost;
}

std::unordered_map<int, std::pair< network::MLCST, std::vector<int> > >
MLCMST_SubnetSolver::allSubnetTrees(const network::MLCCNetwork &network, const std::vector<int> &vertex_subnet)
{
    return allSubnetTrees(network, createGroups(network.center(), vertex_subnet));
}

std::unordered_map<int, double>
MLCMST_SubnetSolver::allSubnetTreeCosts(const network::MLCCNetwork &network, const std::vector<int> &vertex_subnet)
{
    return allSubnetTreeCosts(network, createGroups(network.center(), vertex_subnet));
}

std::unordered_map<int, std::pair<network::MLCST, std::vector<int> > >
MLCMST_SubnetSolver::allSubnetTrees(const network::MLCCNetwork &network,
    const std::unordered_map<int, std::vector<int>> &groups)
{
    auto results = solveAllSubnets(network, groups);
    std::unordered_map<int, std::pair< network::MLCST, std::vector<int> >> trees;
    std::transform(results.begin(), results.end(), std::inserter(trees, trees.end()),
        [] (const auto& p) {
            auto r = p.second;
            return std::make_pair(p.first, std::make_pair(r.mlcmst, r.mapping));
        });
    return trees;
}

std::unordered_map<int, double>
MLCMST_SubnetSolver::allSubnetTreeCosts(const network::MLCCNetwork &network,
    const std::unordered_map<int, std::vector<int>> &groups)
{
    auto results = solveAllSubnets(network, groups);
    std::unordered_map<int, double> costs;
    std::transform(results.begin(), results.end(), std::inserter(costs, costs.end()),
        [] (const auto& p) {
            auto r = p.second;
            return std::make_pair(p.first, r.cost);
        });
    return costs;
}

std::unordered_map<int, MLCMST_SubnetSolver::Result>
MLCMST_SubnetSolver::solveAllSubnets(const network::MLCCNetwork& network, const std::vector<int> &subnet_vertices)
{
    return solveAllSubnets(network, createGroups(network.center(), subnet_vertices));
}

std::unordered_map<int, MLCMST_SubnetSolver::Result>
MLCMST_SubnetSolver::solveAllSubnets(
        const network::MLCCNetwork& network, const std::unordered_map<int, std::vector<int>> &groups)
{
    std::unordered_map<int, MLCMST_SubnetSolver::Result> results;
    std::transform(groups.begin(), groups.end(), std::inserter(results, results.end()),
        [&] (const auto& p) {
            return std::make_pair(p.first, solveSubnet(network, p.second));
        });
    return results;
}

MLCMST_SubnetSolver::Result
MLCMST_SubnetSolver::solveSubnet(const network::MLCCNetwork &network, std::vector<int> subnet_vertices)
{
    if (std::find(subnet_vertices.begin(), subnet_vertices.end(), network.center()) == subnet_vertices.end()) {
        subnet_vertices.push_back(network.center());
    }
    auto [sub_network, mapping] = network.subNetwork(subnet_vertices);
    auto result = solver_->solve(sub_network);

    return Result {
        .cost = result.mlcst.value().cost(sub_network),
        .mlcmst = result.mlcst.value(),
        .mapping = mapping
    };
}

std::unordered_map<int, std::vector<int>>
MLCMST_SubnetSolver::createGroups(int center, const std::vector<int> &vertex_subnet)
{
    std::unordered_map<int, std::vector<int>> groups;
    for (int i=0; i < vertex_subnet.size(); i++) {
        groups[vertex_subnet[i]].push_back(i);
    }
    groups.erase(vertex_subnet[center]);
    return groups;
}

network::MLCST MLCMST_SubnetSolver::solveMLCMST(
        const network::MLCCNetwork &network, const std::vector<int> &vertex_subnet)
{
    return solveMLCMST(network, createGroups(network.center(), vertex_subnet));
}

network::MLCST MLCMST_SubnetSolver::solveMLCMST(
        const network::MLCCNetwork &network, const std::unordered_map<int, std::vector<int>> &groups)
{
    network::MLCST mlcmst(network.vertexCount(), network.center());
    for (const auto& group : groups) {
        auto [ inner_mlcmst, mapping ] = subnetTree(network, group.second);
        for (int i=0; i<mapping.size(); i++) {
            mlcmst.parent(mapping[i]) = mapping[inner_mlcmst.parent(i)];
            mlcmst.facilityLevel(mapping[i]) = inner_mlcmst.facilityLevel(i);
        }
    }
    return mlcmst;
}


}
