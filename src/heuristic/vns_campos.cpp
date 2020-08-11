#include <heuristic/vns_campos.hpp>

#include <stdexcept>

namespace MLCMST::heuristic {

std::string VNS_Campos::id()
{
    static const std::string id = "VNS_campos";
    return id;
}

VNS_Campos::VNS_Campos(std::unique_ptr< MLCMST_Heuristic > init_solver, Params params)
    : init_solver_(std::move(init_solver)), params(params), int_generator_(0,std::numeric_limits<int>::max())
{
    random_neighbour_generator_ = {
        [&] (const network::MLCST& mlcmst, const network::MLCCNetwork& mlcc_network) { return leafSwapNeighbourhood(
                mlcmst, mlcc_network); },
        [&] (const network::MLCST& mlcmst, const network::MLCCNetwork& mlcc_network) { return explodeSubtreeNeighbourhood(
                mlcmst, mlcc_network); },
        [&] (const network::MLCST& mlcmst, const network::MLCCNetwork& mlcc_network) { return gatherSingleNodesNeighbourhood(
                mlcmst, mlcc_network); },
        [&] (const network::MLCST& mlcmst, const network::MLCCNetwork& mlcc_network) { return levelDownRootSubtreeNeighbour(
                mlcmst, mlcc_network); },
        [&] (const network::MLCST& mlcmst, const network::MLCCNetwork& mlcc_network) { return mergeRootSubtreesNeighbour(
                mlcmst,
                mlcc_network); }
    };
}

VNS_Campos::~VNS_Campos() = default;

bool VNS_Campos::isUnitDemand(const network::MLCCNetwork &mlcc_network) {
    for (int i : mlcc_network.regularVertexSet()) {
        if (mlcc_network.demand(i) != 1)
            return false;
    }
    return true;
}

network::MLCST VNS_Campos::run(const network::MLCCNetwork &mlcc_network)
{
    if (!isUnitDemand(mlcc_network))
        throw std::invalid_argument(id() + " only supports unit demand MLCMST problem instances");

    struct BestTree {
        network::MLCST mlcmst;
        double cost;
    };
    const int max_failed_cycles = 100;


    BestTree bestTree{
        .mlcmst = localSearch(init_solver_->run(mlcc_network), mlcc_network),
    };
    bestTree.mlcmst.setMinimalViableLevels(mlcc_network);
    bestTree.cost = bestTree.mlcmst.cost(mlcc_network);

    int failed_cycles = 0;
    while (failed_cycles < max_failed_cycles) {
        for (auto & neighbour_drawer : random_neighbour_generator_) {
            network::MLCST mlcmst = neighbour_drawer(bestTree.mlcmst, mlcc_network);
            mlcmst = localSearch(mlcmst, mlcc_network);
            double cost = mlcmst.cost(mlcc_network);
            if (cost + 1e-9 < bestTree.cost) {
                bestTree = { .mlcmst= mlcmst, .cost = cost };
                failed_cycles = -1;
                break;
            }
        }
        failed_cycles++;
    }

    return bestTree.mlcmst;
}

network::MLCST VNS_Campos::localSearch(network::MLCST mlcmst, const network::MLCCNetwork &mlcc_network)
{
    struct BestImprovement {
        network::MLCST mlcmst;
        double cost;
    };

    bool improving = true;
    while (improving) {
        BestImprovement bestImprovement{ .mlcmst= mlcmst, .cost = mlcmst.cost(mlcc_network) };
        std::vector<int> leafs = mlcmst.leafs();
        improving = false;
        for (int i=0; i<leafs.size(); i++) {
            for (int j=i+1; j<leafs.size(); j++) {
                auto [v,w] = std::make_pair(leafs[i], leafs[j]);
                std::swap(mlcmst.parent(v), mlcmst.parent(w));

                double cost = mlcmst.cost(mlcc_network);
                if (cost + 1e-9 < bestImprovement.cost) {
                    bestImprovement = {.mlcmst= mlcmst, .cost = cost};
                }

                std::swap(mlcmst.parent(v), mlcmst.parent(w));
            }
        }

        if (bestImprovement.cost + 1e-9 < mlcmst.cost(mlcc_network)) {
            mlcmst = bestImprovement.mlcmst;
        }
    }

    return mlcmst;
}

network::MLCST
VNS_Campos::leafSwapNeighbourhood(const network::MLCST &mlcmst, const network::MLCCNetwork &mlcc_network)
{
    std::vector<int> leafs = mlcmst.leafs();
    if (leafs.size() == 1) {
        return mlcmst;
    }
    int v = leafs[int_generator_.generate() % (int)leafs.size()];
    int w = v;
    while (w==v)
        w = leafs[int_generator_.generate() % (int) leafs.size()];

    network::MLCST neighbour = mlcmst;
    std::swap(neighbour.parent(v), neighbour.parent(w));
    return neighbour;
}

network::MLCST
VNS_Campos::explodeSubtreeNeighbourhood(const network::MLCST &mlcmst, const network::MLCCNetwork &mlcc_network)
{
//    maybe add a neighbourhood where exploded subtree nodes are attached to old subtree root's parent instead of tree root
    std::vector<int> non_leaf_terminals = mlcmst.nonLeafTerminals();
    if (non_leaf_terminals.empty())
        return mlcmst;

    int v = non_leaf_terminals[int_generator_.generate() % (int)non_leaf_terminals.size()];
    std::vector<int> v_subtree = mlcmst.subtreeVertices(v);
    network::MLCST neighbour = mlcmst;
    for (int i : v_subtree) {
        neighbour.parent(i) = neighbour.parent(neighbour.root());
//        neighbour.parent(i) = neighbour.parent(mlcmst.parent(v)); maybe in a new neighbourhood ?
    }
    neighbour.setMinimalViableLevels(mlcc_network);
    return neighbour;
}

network::MLCST
VNS_Campos::gatherSingleNodesNeighbourhood(const network::MLCST &mlcmst, const network::MLCCNetwork &mlcc_network)
{
    std::vector<std::vector<int>> children = mlcmst.childrenLists();
    std::vector<int> single_nodes;
    for (int c : children[mlcmst.root()]) {
        if (children[c].empty())
            single_nodes.push_back(c);
    }
    const int max_capacity = mlcc_network.edgeCapacity(mlcc_network.levelsNumber()-1);
    if (single_nodes.size() <= 1 || max_capacity == 1) {
        return mlcmst;
    }

    network::MLCST neighbour = mlcmst;
    int C = (int_generator_.generate() % std::min(max_capacity, (int)single_nodes.size())) + 1;
    if (C==1) C++;
    std::shuffle(single_nodes.begin(), single_nodes.end(), random_engine_);
    const int subtree_center = single_nodes[0];
    neighbour.parent(subtree_center) = neighbour.root();
    for (int i=1; i<C && i < single_nodes.size(); i++) {
        neighbour.parent(single_nodes[i]) = subtree_center;
    }

    neighbour.setMinimalViableLevels(mlcc_network);
    return neighbour;
}

network::MLCST
VNS_Campos::levelDownRootSubtreeNeighbour(const network::MLCST &mlcmst, const network::MLCCNetwork &mlcc_network)
{
//    maybe add neighbourhood where not only root subtrees are considered, by all of the subtrees
    auto children = mlcmst.childrenLists();
    std::vector<int> root_non_leaf_children = children[mlcmst.root()];
    for (int i=0; i < root_non_leaf_children.size(); i++) {
        if (children[root_non_leaf_children[i]].empty()) {
            std::swap(root_non_leaf_children[i], root_non_leaf_children.back());
            root_non_leaf_children.pop_back();
            i--;
        }
    }
    if (root_non_leaf_children.empty())
        return mlcmst;
    int v = (int_generator_.generate() % (int)root_non_leaf_children.size());

    network::MLCST neighbour = mlcmst;
    for (int c : children[v]) {
        neighbour.parent(c) = neighbour.root();
    }
    neighbour.setMinimalViableLevels(mlcc_network);
    return neighbour;
}

network::MLCST
VNS_Campos::mergeRootSubtreesNeighbour(const network::MLCST &mlcmst, const network::MLCCNetwork &mlcc_network)
{
//    maybe add neighbourhood where not only root children are merged or

    const int min_capacity = mlcc_network.edgeCapacity(0);
    const int max_capacity = mlcc_network.edgeCapacity(mlcc_network.levelsNumber()-1);
    if (max_capacity == 1)
        return mlcmst;

    // maybe in a new neighbourhood capacity random?
//    const int C = (int_generator_.generate() % (max_capacity - min_capacity)) + min_capacity;
    const int C = max_capacity;
    std::vector<int> root_children = mlcmst.childrenLists()[mlcmst.root()];
    std::vector<int> loads = mlcmst.loads(mlcc_network);
    std::shuffle(root_children.begin(), root_children.end(), random_engine_);
    std::vector<int> subtrees_to_merge;
    int new_subtree_load = 0;
    for (int v : root_children) {
        if (new_subtree_load + loads[v] <= C) {
            new_subtree_load += loads[v];
            subtrees_to_merge.push_back(v);
        }
    }
    if (subtrees_to_merge.empty()) {
        return mlcmst;
    }
    int subtree_root = subtrees_to_merge[0];

    network::MLCST neighbour = mlcmst;
    for (int v : subtrees_to_merge) {
        neighbour.parent(v) = subtree_root;
    }
    neighbour.parent(subtree_root) = mlcmst.root();
    neighbour.setMinimalViableLevels(mlcc_network);
    return neighbour;
}

}
