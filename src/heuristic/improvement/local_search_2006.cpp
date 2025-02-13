#include <heuristic/improvement/local_search_2006.hpp>

#include <algorithm>
#include <chrono>
#include <numeric>
#include <queue>
#include <stack>
#include <stdexcept>

#include <heuristic/star.hpp>

namespace MLCMST::heuristic::improvement {

std::string LocalSearch2006::id()
{
    static std::string id = "local_search_2006";
    return id;
}

double LocalSearch2006::EPS_ = 1e-9;

LocalSearch2006::LocalSearch2006(std::unique_ptr< MLCMST_Solver > subnet_solver, Params params)
    : LocalSearch2006(std::make_unique<Star>(), std::move(subnet_solver), params)
{
}

LocalSearch2006::LocalSearch2006(std::unique_ptr< MLCMST_Solver > init_solver,
                                 std::unique_ptr< MLCMST_Solver > subnet_solver, Params params)
    : MLCMST_Improver(std::move(init_solver)),
    params_(params), subnet_solver_(std::move(subnet_solver)), random_int_generator_(0, std::numeric_limits<int>::max())
{
}

LocalSearch2006::~LocalSearch2006() = default;


network::MLCST LocalSearch2006::improve(long steps, network::MLCST mlcmst, const network::MLCCNetwork &mlcc_network)
{
    network_ = &mlcc_network;

    std::vector<int> group_ids = mlcmst.subnet();
    while (steps > 0) {
        steps--;
        std::vector<int> new_group_ids = improvementStep(*network_, group_ids);
        if (std::equal(group_ids.begin(), group_ids.end(), new_group_ids.begin())) {
            break;
        }
        group_ids = new_group_ids;
    }

    return subnet_solver_.solveMLCMST(*network_, group_ids);
}

std::vector<int>
LocalSearch2006::improvementStep(const network::MLCCNetwork &network, std::vector<int> group_ids)
{
    network_ = &network;
    std::optional< std::vector<int> > exchange = findBestProfitableExchange(group_ids);
    if (exchange.has_value()) {
        implementExchange(exchange.value(), group_ids);
    }
    return group_ids;
}

void LocalSearch2006::implementExchange(std::vector<int> exchange, std::vector<int> &group_id)
{
    const int N = group_id.size();
    std::pair<int,int> close;
    if (exchange.back() >= N) {
        int g_id = exchange.back()-N;
        exchange.pop_back();
        close= std::make_pair(exchange.back(), g_id);
    } else {
        throw std::logic_error("Unexpected error, should be >= N");
    }
    for (int i=0; i < exchange.size()-1; i++) {
        int v = exchange[i], w = exchange[i+1];
        group_id[v] = group_id[w];
    }
    auto [ v, g ] = close;
    group_id[v] = g;
}

network::Network LocalSearch2006::buildNeighbourhoodGraph(const std::vector<int> &group_id)
{
    network::Network network(2*network_->vertexCount() + 1);
    const int N = network_->vertexCount();
    const int origin = 2*N;
    const int max_edge_capacity = network_->edgeCapacity(network_->levelsNumber()-1);
    const auto subnetGroups = this->groups(group_id);
    const auto subnetCosts = subnet_solver_.allSubnetTreeCosts(*network_, subnetGroups);
    const std::unordered_map<int, int> group_demands = groupDemands(group_id);

    // regular node edges
    for (int i : network_->terminalVertexSet()) {
        for (int j : network_->terminalVertexSet()) {
            if (i==j || group_id[i] == group_id[j])
                continue;
            if (group_demands.at(group_id[j]) + network_->demand(i) - network_->demand(j) > max_edge_capacity)
                continue;
            std::vector<int> new_group = subnetGroups.at(group_id[j]);
            new_group.erase(std::find(new_group.begin(), new_group.end(), j));
            new_group.push_back(i);
            network.edgeCost(i, j) = subnet_solver_.subnetTreeCost(*network_, new_group) - subnetCosts.at(group_id[j]);
        }
    }

    // pseudo node edges
    for (int i : network_->terminalVertexSet()) {
        for (const auto& group : subnetGroups) {
            int g_id = group.first;
            if (group_id[i] == g_id)
                continue;
            if (group_demands.at(g_id) + network_->demand(i) > max_edge_capacity)
                continue;
            std::vector<int> new_group = group.second;
            new_group.push_back(i);
            network.edgeCost(i, N + g_id) = subnet_solver_.subnetTreeCost(*network_, new_group) - subnetCosts.at(g_id);
        }
    }

    // origin edges
    for (const auto& group : subnetGroups) {
        int g_id = group.first;
        network.edgeCost(N + g_id, origin) = 0;
    }
    for (int i : network_->terminalVertexSet()) {
        std::vector<int> new_group = subnetGroups.at(group_id[i]);
        new_group.erase(std::find(new_group.begin(), new_group.end(), i));
        network.edgeCost(origin, i) = subnet_solver_.subnetTreeCost(*network_, new_group) - subnetCosts.at(group_id[i]);
    }

    return network;
}

std::optional<std::vector<int>> LocalSearch2006::findBestProfitableExchange(const std::vector<int> &group_id)
{
    network::Network graph = buildNeighbourhoodGraph(group_id);
    std::pair<std::vector<int>, double> best_exchange = std::make_pair(std::vector<int>{}, 0);
    std::vector<int> starting_vertices = network_->terminalVertexSet();
    if (!params_.cycle_search_iterate_all) {
        int s = starting_vertices[random_int_generator_.generate() % starting_vertices.size()];
        starting_vertices = { s };
    }

    for (int i : starting_vertices) {
        auto maybe_exchange = findBestProfitableExchange(i, graph, group_id);
        if (maybe_exchange.has_value() && maybe_exchange.value().second + EPS_ < best_exchange.second) {
            best_exchange = maybe_exchange.value();
        }
    }

    return best_exchange.second < -EPS_ ? std::optional(best_exchange.first) : std::nullopt;
}

std::optional<std::pair<std::vector<int>, double>>
LocalSearch2006::findBestProfitableExchange(int s, const network::Network &net, const std::vector<int> &group_id)
{
    const int N = group_id.size();
    const int origin = 2*N;
    const std::vector<std::vector<int>> graph = net.neighbourhoods();

    double cost = 0;
    std::vector<int> exchange;

    std::vector<double> d(N, std::numeric_limits<double>::max());
    std::vector<int> pred(N);
    std::queue<int> fifo;

    d[s] = 0;
    pred[s] = s;
    fifo.push(s);

    auto getPath = [&] (int v) {
        std::vector<int> path;
        do {
            path.push_back(v);
            v = pred[v];
        } while (path.back() != s);
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
        bool continue_search = (d[j] > d[i] + net.edgeCost(i, j));

        std::vector<int> path = getPath(i);
        std::vector<int> groups;
        std::transform(path.begin(), path.end(), std::back_inserter(groups), [&](int v) { return group_id[v]; });

        if (j < N) { // regular
            if (std::find(path.begin(), path.end(), j) != path.end()) { // found cycle {j - path.back()} - cyclic exchange
                path.erase(path.begin(), std::find(path.begin(), path.end(), j));
                double cost = getCost(path) + net.edgeCost(i,j);
                path.push_back(N+group_id[j]);
                update(path, cost);
            } else if (std::find(groups.begin(), groups.end(), group_id[j]) == groups.end() && continue_search) { // found path {s - j}
                d[j] = d[i] + net.edgeCost(i, j);
                pred[j] = i;
                fifo.push(j);
            }
        } else { // pseudo node
            const int g_id = j-N;
            if (std::find(groups.begin(), groups.end(), g_id) == groups.end()) { // found disjoint cycle - path exchange
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
