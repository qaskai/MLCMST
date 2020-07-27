#include <mp/util.hpp>

#include <util/util.hpp>

namespace MLCMST::mp::util
{

operations_research::LinearExpr createDefaultObjectiveExpression(
    const network::MLCCNetwork& mlcc_network,
    const std::vector<std::vector<std::vector<operations_research::LinearExpr>>>& arc_vars
)
{
    operations_research::LinearExpr expr;
    for (int i=0; i < mlcc_network.vertexCount(); i++) {
        for (int j=0; j < mlcc_network.vertexCount(); j++) {
            if (i == j)
                continue;
            for (int l=0; l < mlcc_network.levelsNumber(); l++) {
                double cost = mlcc_network.network(l).edgeCost(i, j);
                expr += cost * arc_vars[i][j][l];
            }
        }
    }
    return expr;
}

network::MLCMST createMLCMST(
    const network::MLCCNetwork& mlcc_network,
    const std::vector<std::vector<std::vector<operations_research::LinearExpr>>>& arc_vars
)
{
    network::MLCMST mlcmst(mlcc_network.vertexCount(), mlcc_network.center());
    for (int i=0; i < mlcc_network.vertexCount(); i++) {
        for (int j=0; j < mlcc_network.vertexCount(); j++) {
            for (int l=0; l < mlcc_network.levelsNumber(); l++) {
                auto var = arc_vars[i][j][l].terms().begin()->first;
                if (var->solution_value() > 0.99) {
                    mlcmst.parent(i) = j;
                    mlcmst.edgeLevel(i) = l;
                    goto found_parent;
                }
            }
        }
        found_parent:;
    }
    return mlcmst;
}

std::vector<operations_research::LinearExpr> variablesToExpr(const std::vector<operations_research::MPVariable*>& vars)
{
    std::vector<operations_research::LinearExpr> expr_vec;
    for (operations_research::MPVariable* v : vars) {
        expr_vec.emplace_back(v);
    }
    return expr_vec;
}

std::vector<std::tuple<int, int>> createArcSet(const network::MLCCNetwork& mlcc_network)
{
    std::vector<std::tuple<int,int>> arc_set;
    auto neighbourhood_lists = mlcc_network.network(0).neighbourhoods();
    for (int v : MLCMST::util::firstN(mlcc_network.vertexCount())) {
        for (int w : neighbourhood_lists[v]) {
                arc_set.emplace_back(v, w);
        }
    }
    return arc_set;
}

std::vector<std::tuple<int, int>> createUndirectedEdgeSet(const network::MLCCNetwork& mlcc_network)
{
    std::set<std::tuple<int, int>> undirected_edges;
    for (auto [v,w] : createArcSet(mlcc_network)) {
        if (v > w)
            std::swap(v,w);
        undirected_edges.insert({ v,w });
    }
    return std::vector<std::tuple<int,int>>(undirected_edges.begin(), undirected_edges.end());
}


}
