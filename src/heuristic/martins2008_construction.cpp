#include <heuristic/martins2008_construction.hpp>

#include <cassert>
#include <limits>
#include <iterator>
#include <util/util.hpp>

namespace MLCMST::heuristic {

std::string Martins2008_Construction::id()
{
    static const std::string id = "martins2008_construction";
    return  id;
}



Martins2008_Construction::Martins2008_Construction(std::unique_ptr<MLCMST_Solver> subnet_solver, Params params)
    : Martins2008_Construction(std::move(subnet_solver), util::clockMilliseconds(), params)
{
}

Martins2008_Construction::Martins2008_Construction(std::unique_ptr<MLCMST_Solver> subnet_solver, long seed, Params params)
    :  params(params), subnet_solver_(std::move(subnet_solver)), int_generator_(0, std::numeric_limits<int>::max(), seed)
{
}

Martins2008_Construction::~Martins2008_Construction() = default;

network::MLCST Martins2008_Construction::run(const network::MLCCNetwork &mlcc_network)
{
    mlcc_network_ = &mlcc_network;

    std::vector<int> vertex_group_id(mlcc_network.vertexCount());
    vertex_group_id[mlcc_network.center()] = mlcc_network.vertexCount()-1;

    std::set<int> unassigned;
    {
        auto terminals = mlcc_network.regularVertexSet();
        unassigned.insert(terminals.begin(), terminals.end());
    }

    int id = 0;
    while (!unassigned.empty()) {
        std::vector<int> group = createNewGroup(unassigned);
        for (int v: group) {
            vertex_group_id[v] = id;
            unassigned.erase(v);
        }
        id++;
    }
    network::MLCST mlcmst =  subnet_solver_.solveMLCMST(mlcc_network, vertex_group_id);
    return mlcmst;
}

std::vector<int> Martins2008_Construction::createNewGroup(const std::set<int>& unassigned)
{
    std::vector<int> group;
    std::unordered_map< int, double > distance;
    {
        int i = *std::next(unassigned.begin(), int_generator_.generate() % unassigned.size());
        for (int x : unassigned) {
            distance[x] = mlcc_network_->edgeCost(x, i, 0);
        }
        distance.erase(i);
        group.push_back(i);
    }

    while (group.size() < params.subnet_size && !distance.empty()) {
        auto [d_min_it, d_max_it] = std::minmax_element(distance.begin(), distance.end(),
        [] (const auto& p1, const auto& p2) {
                return p1.second < p2.second;
            });
        auto [d_min, d_max] = std::make_pair(d_min_it->second, d_max_it->second);

        std::vector<int> RCL;
        double threshold = d_min + params.alpha * (d_max - d_min);
        for (auto [idx, d] : distance) {
            if (d < threshold + 1e-9) {
                RCL.push_back(idx);
            }
        }

        int j = RCL[int_generator_.generate() % RCL.size()];
        group.push_back(j);
        distance.erase(j);

        for (auto& [idx, dist] : distance) {
            dist = std::min(dist, mlcc_network_->edgeCost(j, idx, 0));
        }
    }

    return group;
}

}
