#include <heuristic/local_search_2006.hpp>

#include <algorithm>
#include <chrono>
#include <numeric>
#include <queue>

#include <heuristic/link_upgrade_ud.hpp>

namespace MLCMST::heuristic {

double LocalSearch2006::EPS_ = 1e-9;

LocalSearch2006::LocalSearch2006() : LocalSearch2006(std::make_unique<LinkUpgradeUD>())
{

}

LocalSearch2006::LocalSearch2006(std::unique_ptr<MLCMSTSolver> inner_mlcmst_solver)
    : inner_mlcmst_solver_(std::move(inner_mlcmst_solver))
{

}

LocalSearch2006::~LocalSearch2006() = default;

MLCMSTSolver::Result LocalSearch2006::solve(const network::MLCCNetwork &network)
{
    auto time_start = std::chrono::high_resolution_clock::now();
    network_ = &network;
    std::vector<int> group_id = inner_mlcmst_solver_->solve(network).mlcmst->subnet();

    while ( step(group_id) );

    auto time_end = std::chrono::high_resolution_clock::now();
    double wall_time = std::chrono::duration<double, std::milli>(time_end - time_start).count();

    return Result {
        buildMLCMST(group_id),
        std::nullopt,
        wall_time,
        true
    };
}

bool LocalSearch2006::step(std::vector<int>& group_id)
{
    std::optional< std::vector<int> > exchange = findBestProfitableExchange(group_id);
    if (exchange.has_value()) {
        implementExchange(exchange.value(), group_id);
        return true;
    }
    else {
        return false;
    }
}

void LocalSearch2006::implementExchange(std::vector<int> exchange, std::vector<int> &group_id)
{
    const int N = group_id.size();
    std::optional<std::pair<int,int>> close;
    if (exchange.back() >= N) {
        int g_id = exchange.back()-N;
        exchange.pop_back();
        close= std::make_pair(exchange.back(), g_id);
    }
    for (int i=0; i < exchange.size()-1; i++) {
        int v = exchange[i], w = exchange[i+1];
        group_id[v] = group_id[w];
    }
    if (close.has_value()) {
        auto [ v, g ] = close.value();
        group_id[v] = g;
    }
}

LocalSearch2006::Network LocalSearch2006::buildNeighbourhoodGraph(const std::vector<int> &group_id)
{
    Network network(2*network_->vertexCount() + 1);
    const int N = network_->vertexCount();
    const int origin = 2*N;
    const int max_edge_capacity = network_->edgeCapacity(network_->levelsNumber()-1);
    const auto subnetCosts = this->allSubnetCosts(group_id);
    const auto subnetGroups = this->groups(group_id);
    const std::unordered_map<int, int> group_demands = groupDemands(group_id);

    // regular node edges
    for (int i : network_->regularVertexSet()) {
        for (int j : network_->regularVertexSet()) {
            if (i==j || group_id[i] == group_id[j])
                continue;
            if (group_demands.at(group_id[j]) + network_->demand(i) - network_->demand(j) > max_edge_capacity)
                continue;
            std::vector<int> new_group = subnetGroups.at(group_id[j]);
            new_group.erase(std::find(new_group.begin(), new_group.end(), j));
            new_group.push_back(i);
            network.edgeCost(i, j) = subnetCost(new_group) - subnetCosts.at(group_id[j]);
        }
    }

    // pseudo node edges
    for (int i : network_->regularVertexSet()) {
        for (const auto& group : subnetGroups) {
            int g_id = group.first;
            if (group_id[i] == g_id)
                continue;
            if (group_demands.at(g_id) + network_->demand(i) > max_edge_capacity)
                continue;
            std::vector<int> new_group = group.second;
            new_group.push_back(i);
            network.edgeCost(i, N + g_id) = subnetCost(new_group) - subnetCosts.at(g_id);
        }
    }

    // origin edges
    for (const auto& group : subnetGroups) {
        int g_id = group.first;
        network.edgeCost(N + g_id, origin) = 0;
    }
    for (int i : network_->regularVertexSet()) {
        std::vector<int> new_group = subnetGroups.at(group_id[i]);
        new_group.erase(std::find(new_group.begin(), new_group.end(), i));
        network.edgeCost(origin, i) = subnetCost(new_group) - subnetCosts.at(group_id[i]);
    }

    return network;
}

LocalSearch2006::MLCMST LocalSearch2006::buildMLCMST(const std::vector<int> &group_id)
{
    MLCMST mlcmst(network_->vertexCount(), network_->center());
    for (const auto& group : groups(group_id)) {
        auto [ inner_mlcmst, mapping, _ ] = solveSubnet(group.second);
        for (int i=0; i<mapping.size(); i++) {
            mlcmst.parent(mapping[i]) = mapping[inner_mlcmst.parent(i)];
            mlcmst.edgeLevel(mapping[i]) = inner_mlcmst.edgeLevel(i);
        }
    }
    return mlcmst;
}

std::optional<std::vector<int>> LocalSearch2006::findBestProfitableExchange(const std::vector<int> &group_id)
{
    Network graph = buildNeighbourhoodGraph(group_id);
    std::pair<std::vector<int>, double> best_exchange = std::make_pair(std::vector<int>{}, 0);

    for (int i : network_->regularVertexSet()) {
        auto maybe_exchange = findBestProfitableExchange(i, graph, group_id);
        if (maybe_exchange.has_value() && maybe_exchange.value().second < best_exchange.second) {
            best_exchange = maybe_exchange.value();
        }
    }

    return best_exchange.second < -EPS_ ? std::optional(best_exchange.first) : std::nullopt;
}

std::optional<std::pair<std::vector<int>, double>>
LocalSearch2006::findBestProfitableExchange(int s, const LocalSearch2006::Network &net, const std::vector<int> &group_id)
{
    const int N = group_id.size();
    const int origin = 2*N;
    const std::vector<std::vector<int>> graph = net.neighbourhoods();

    double cost = 0;
    std::vector<int> exchange;

    std::vector<double> d(N, std::numeric_limits<double>::infinity());
    d[s] = 0;
    std::vector<int> pred(N);
    pred[s] = s;
    std::queue<int> fifo;
    fifo.push(s);

    auto getPath = [&] (int v) {
        std::vector<int> path;
        while (v != s) {
            path.push_back(v);
            v = pred[v];
        }
        path.push_back(s);
        std::reverse(path.begin(), path.end());
        return path;
    };

    auto checkUnique = [&] (std::vector<int> v) -> bool {
        std::sort(v.begin(), v.end());
        for (int i=0; i<v.size()-1; i++) {
            if (v[i] == v[i+1])
                return false;
        }
        return true;
    };

    auto getCost = [&] (std::vector<int> path) -> double {
        double c = 0;
        for (int i=0; i<path.size()-1; i++) {
            c += net.edgeCost(path[i], path[i+1]);
        }
        return c;
    };

    auto update = [&] (const std::vector<int>& maybe_exchange, double c) {
        if (cost > c + EPS_) {
            exchange = maybe_exchange;
            cost = c;
        }
    };

    auto process_arc = [&] (int i, int j) {
        if (d[j] <= d[i] + net.edgeCost(i, j) + EPS_) {
            return;
        }
        std::vector<int> path = getPath(i);
        std::vector<int> groups;
        std::transform(path.begin(), path.end(), std::back_inserter(groups), [&](int v) { return group_id[v]; });

        if (j < N) { // regular
            if (std::find(path.begin(), path.end(), j) != path.end()) { // found cycle {j - path.back()}
                path.erase(path.begin(), std::find(path.begin(), path.end(), j));
                double cost = getCost(path) + net.edgeCost(i,j);
                path.push_back(N+group_id[j]);
                update(path, cost);
            } else if (std::find(groups.begin(), groups.end(), group_id[j]) == groups.end()) { // found path {s - j}
                d[j] = d[i] + net.edgeCost(i, j);
                pred[j] = i;
                fifo.push(j);
            }
        } else { // pseudo node
            const int g_id = j-N;
            if (std::find(groups.begin(), groups.end(), g_id) == groups.end()) { // found disjoint path
                path.push_back(j);
                update(path, getCost(path) + net.edgeCost(origin, s));
            }
        }
    };

    while (!fifo.empty()) {
        int i = fifo.front();
        fifo.pop();
        std::vector<int> path = getPath(i);
        std::vector<int> groups;
        std::transform(path.begin(), path.end(), std::back_inserter(groups), [&](int v) { return group_id[v]; });
        if (!checkUnique(groups))
            continue;
        d[i] = getCost(path);

        for (int j : graph[i]) {
            process_arc(i, j);
        }
    }

    return cost < -EPS_ ? std::optional(std::make_pair(exchange, cost)) : std::nullopt;
}

std::unordered_map<int, std::vector<int>> LocalSearch2006::groups(const std::vector<int> &group_id)
{
    std::unordered_map<int, std::vector<int>> groups;
    for (int i = 0; i < group_id.size(); i++) {
        groups[group_id[i]].push_back(i);
    }
    if (groups.size() < group_id.size()) {
        int x = 0;
        while (groups.count(x))
            x++;
        groups[x] = {};
    }
    groups.erase(group_id[network_->center()]);
    return groups;
}

std::unordered_map<int, double> LocalSearch2006::allSubnetCosts(const std::vector<int> &group_id)
{
    std::unordered_map<int, double> costs;
    for (const auto& group : groups(group_id)) {
        costs[group.first] = subnetCost(group.second);
    }
    return costs;
}

double LocalSearch2006::subnetCost(const std::vector<int> &vertices)
{
    double cost;
    std::tie(std::ignore, std::ignore, cost) = solveSubnet(vertices);
    return cost;
}

std::tuple<network::MLCMST, std::vector<int>, double> LocalSearch2006::solveSubnet(const std::vector<int>& vertices)
{
    auto [ sub_net, mapping ] = network_->subNetwork(vertices);
    MLCMST sub_net_mlcmst = inner_mlcmst_solver_->solve(sub_net).mlcmst.value();
    return std::make_tuple(sub_net_mlcmst, mapping, sub_net_mlcmst.cost(sub_net));
}

std::unordered_map<int, int> LocalSearch2006::groupDemands(const std::vector<int>& group_id) {
    std::unordered_map<int, int> demands;
    for (const auto& g : groups(group_id)) {
        const int g_id = g.first;
        demands[g_id] = 0;
        for (int v : g.second) {
            demands[g_id] += network_->demand(v);
        }
    }
    return demands;
}

}
