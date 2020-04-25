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
    std::vector<int> parents(mlcc_network.vertexCount()), edge_level(mlcc_network.vertexCount());
    for (int i=0; i < mlcc_network.vertexCount(); i++) {
        for (int j=0; j < mlcc_network.vertexCount(); j++) {
            for (int l=0; l < mlcc_network.levelsNumber(); l++) {
                auto var = arc_vars[i][j][l].terms().begin()->first;
                if (var->solution_value() > 0.99) {
                    parents[i] = j;
                    edge_level[i] = l;
                    goto found_parent;
                }
            }
        }
        found_parent:;
    }
    return network::MLCMST(mlcc_network, parents, edge_level);
}

std::vector<operations_research::LinearExpr> variablesToExpr(const std::vector<operations_research::MPVariable*>& vars)
{
    std::vector<operations_research::LinearExpr> expr_vec;
    for (operations_research::MPVariable* v : vars) {
        expr_vec.emplace_back(v);
    }
    return expr_vec;
}

std::vector<std::tuple<int, int>> createArcSet(unsigned int N)
{
    std::vector<std::tuple<int,int>> arc_set;
    for (int v : MLCMST::util::firstN(N)) {
        for (int w : MLCMST::util::firstN(N)) {
            if (v != w)
                arc_set.emplace_back(v, w);
        }
    }
    return arc_set;
}

std::vector<std::tuple<int, int>> createUndirectedEdgeSet(unsigned int N)
{
    std::vector<std::tuple<int, int>> undirected_edges;
    for (unsigned int i : MLCMST::util::firstN(N)) {
        for (unsigned int j =i+1; j < N; j++) {
            undirected_edges.emplace_back(i,j);
        }
    }
    return undirected_edges;
}


}
