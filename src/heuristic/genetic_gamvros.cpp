#include <heuristic/genetic_gamvros.hpp>

#include <chrono>
#include <unordered_map>
#include <unordered_set>
#include <queue>

#include <heuristic/improvement/link_upgrade.hpp>
#include <heuristic/improvement/local_search_2006.hpp>

#include <util/number/int_generator.hpp>
#include <util/number/real_number_generator.hpp>
#include <util/number/non_uniform_int_generator.hpp>
#include <util/util.hpp>

#include <iostream>

namespace MLCMST::heuristic {

namespace internal {

Chromosome::Chromosome(int center, const std::vector<int> &vertex_group)
    : center(center), vertex_group(vertex_group), group_ids(createGroupIdVector(center, vertex_group))
{
}

std::vector<int> Chromosome::createGroupIdVector(int center, const std::vector<int> &vertex_group)
{
    std::unordered_set<int> group_ids;
    for (int i=0; i < vertex_group.size(); i++) {
        if (i == center)
            continue;
        group_ids.insert(vertex_group[i]);
    }
    return std::vector<int>(group_ids.begin(), group_ids.end());
}

int Chromosome::freeId() const
{
    std::unordered_set<int> taken(group_ids.begin(), group_ids.end());
    taken.insert(vertex_group[center]);
    for (int i=0; ; i++){
        if (!taken.count(i)) {
            return i;
        }
    }
}

bool Chromosome::hasVertexWithId(int id) const
{
    bool found = false;
    for (int i=0; i<vertex_group.size(); i++) {
        if (i == center)
            continue;
        if (vertex_group[i] == id) {
            found = true;
            break;
        }
    }
    return found;
}

Chromosome Chromosome::refresh() const
{
    std::unordered_map<int, int> mapping;
    std::vector<int> new_vertex_group(vertex_group.size());
    for (int i=0; i<vertex_group.size(); i++) {
        int old_g = vertex_group[i];
        if (!mapping.count(old_g)) {
            mapping[old_g] = i;
        }
        new_vertex_group[i] = mapping[old_g];
    }
    return Chromosome(center, new_vertex_group);
}


bool operator==(const Chromosome& c1, const Chromosome& c2)
{
    if (c1.center != c2.center || c1.vertex_group.size() != c2.vertex_group.size() || c1.group_ids.size() != c2.group_ids.size())
        return false;
    Chromosome c1_fresh = c1.refresh();
    Chromosome c2_fresh = c2.refresh();
    for (int i=0; i < c1_fresh.vertex_group.size(); i++) {
        if (c1_fresh.vertex_group[i] != c2_fresh.vertex_group[i])
            return false;
    }
    return true;
}


bool operator!=(const Chromosome& c1, const Chromosome& c2)
{
    return !(c1 == c2);
}

}


std::string GeneticGamvros::id()
{
    static std::string id = "genetic_gamvros";
    return id;
}


GeneticGamvros::GeneticGamvros(std::vector<std::unique_ptr<MLCMST_Solver>> init_population_solvers,
                               std::unique_ptr<MLCMST_Solver> subnet_solver, const Params& params)
    : params_(params), init_population_solvers_(std::move(init_population_solvers)),
      subnet_solver_(std::move(subnet_solver))
{
}

GeneticGamvros::~GeneticGamvros() = default;

double const GeneticGamvros::EPS_ = 1e-9;

network::MLCST GeneticGamvros::run(const network::MLCCNetwork &mlcc_network)
{
    network_ = &mlcc_network;

    std::vector< internal::Chromosome > population = forceDiversity(initializePopulation());
    for (int i=0; i < params_.generations_number; i++) {
        auto population_with_fitness = evaluateFitness(population);
        auto copied_chromosomes = selectChromosomes(
                params_.population_size - params_.parents_number - params_.most_fit_mutate_number, population_with_fitness);
        auto children = crossoverChromosomes(selectChromosomes(params_.parents_number, population_with_fitness));
        auto mutated_chromosomes = mutate(selectMostFit(
                params_.most_fit_mutate_number, population_with_fitness));

        std::vector< internal::Chromosome > new_population;
        new_population.reserve(params_.population_size);
        new_population.insert(new_population.end(), copied_chromosomes.begin(), copied_chromosomes.end());
        new_population.insert(new_population.end(), children.begin(), children.end());
        new_population.insert(new_population.end(), mutated_chromosomes.begin(), mutated_chromosomes.end());


        population = forceDiversity(new_population);
    }
    internal::Chromosome most_fit_chromosome = selectMostFit(1, evaluateFitness(population))[0];

    return subnet_solver_.solveMLCMST(*network_, most_fit_chromosome.vertex_group);
}

std::vector<internal::Chromosome> GeneticGamvros::initializePopulation()
{
    std::vector<internal::Chromosome> population;
    population.reserve(params_.population_size);
    for (int i=0; i < init_population_solvers_.size() && population.size() < params_.population_size; i++) {
        network::MLCST mlcmst = init_population_solvers_[i]->solve(*network_).mlcst.value();
        population.emplace_back(network_->center(), mlcmst.subnet());
    }
    while (population.size() < params_.population_size) {
        for (int i=0; i < init_population_solvers_.size() && population.size() < params_.population_size; i++) {
            network::MLCCNetwork fuzzed_network = fuzzMLCCNetwork(*network_);
            network::MLCST mlcmst = init_population_solvers_[i]->solve(fuzzed_network).mlcst.value();
            population.emplace_back(network_->center(), mlcmst.subnet());
        }
    }

    return population;
}

network::MLCCNetwork GeneticGamvros::fuzzMLCCNetwork(const network::MLCCNetwork& mlcc_network) const
{
    const int N = mlcc_network.vertexCount();
    util::number::RealNumberGenerator epsilon_generator(
            1.0 - params_.network_fuzzing_epsilon, 1.0 + params_.network_fuzzing_epsilon);

    auto networks = mlcc_network.networks();
    for (int i=0; i<N; i++) {
        for (int j=0; j<N; j++) {
            double eps = epsilon_generator.generate();
            for (network::CapacitatedNetwork& cap_net : networks) {
                double cost = cap_net.edgeCost(i, j);
                cap_net.edgeCost(i,j) = cap_net.edgeCost(j,i) = eps * cost;
            }
        }
    }

    return network::MLCCNetwork(mlcc_network.center(), networks, mlcc_network.demands());
}

std::vector<internal::Chromosome> GeneticGamvros::forceDiversity(std::vector<internal::Chromosome> population)
{
    for (int i=0; i<population.size(); i++) {
        internal::Chromosome c = population[i];
        auto last_it = population.begin() + i;
        while (std::find(population.begin(), last_it, c) != last_it) {
            c = diversify(c);
        }
        population[i] = c;
    }
    return population;
}

internal::Chromosome GeneticGamvros::diversify(internal::Chromosome c)
{
    util::number::IntGenerator int_gen(0, std::numeric_limits<int>::max());
    util::number::RealNumberGenerator probability_gen(0, 1);
    const double create_new_group_prob = 0.9;

    int i = c.center;
    while (i == c.center) {
        i = int_gen.generate() % (int)c.vertex_group.size();
    }
    const int old_g = c.vertex_group[i];
    if (probability_gen.generate() > create_new_group_prob && c.vertex_group.size() > c.group_ids.size() + 1) {
        // create a new group
        c.vertex_group[i] = c.freeId();
    } else {
        int closest_v = closestTerminalInDifferentGroup(i, c.vertex_group, *network_);
        if (closest_v != network_->center()) {
            c.vertex_group[i] = c.vertex_group[closest_v];
        }
    }

    return c.refresh();
}

internal::Chromosome
GeneticGamvros::crossoverChromosomes(const internal::Chromosome &parent1, const internal::Chromosome &parent2)
{
    util::number::IntGenerator int_generator_1(0, parent1.group_ids.size());
    util::number::IntGenerator int_generator_2(0, parent2.group_ids.size());
    util::number::RealNumberGenerator probability_generator(0,1);

    const double move_p = params_.crossover_shrunk_move_probability;
    const int move_k = params_.crossover_move_less_than_k;

    auto generate_dividers = [] (util::number::IntGenerator& generator) {
        int a = generator.generate(), b = generator.generate();
        if (a > b)
            std::swap(a,b);
        return std::make_pair(a,b);
    };
    auto getFirstNFreeIds = [] (int N, const std::vector<int>& vertex_group_ids) {
        std::vector<bool> taken(N+vertex_group_ids.size(), false);
        for (int x : vertex_group_ids)
            taken[x] = true;
        std::vector<int> freeIds;
        for (int i=0; freeIds.size() < N; i++) {
            if (!taken[i]) {
                freeIds.push_back(i);
            }
        }
        return freeIds;
    };

    internal::Chromosome child = parent2;

    std::pair<int,int> dividers1 = generate_dividers(int_generator_1);
    std::pair<int,int> dividers2 = generate_dividers(int_generator_2);
    if (dividers1.first == dividers2.second) {
        if (dividers1.first > 0) {
            dividers1.first--;
        } else {
            dividers1.second++;
        }
    }

    // phase 1 -- move override grouping int parent2 with selected groups of parent1
    std::unordered_map<int, int> id_mapping;
    {
        std::vector<int> moved_groups(
                parent1.group_ids.begin() + dividers1.first, parent1.group_ids.begin() + dividers1.second);
        std::vector<int> new_ids = getFirstNFreeIds(moved_groups.size(), parent2.vertex_group);
        for (int i = 0; i < new_ids.size(); i++) {
            id_mapping[moved_groups[i]] = new_ids[i];
        }
        child.group_ids.insert(child.group_ids.end(), moved_groups.begin(), moved_groups.end());
    }
    std::transform(parent1.group_ids.begin() + dividers1.first, parent1.group_ids.begin() + dividers1.second,
            std::inserter(child.group_ids, child.group_ids.begin() + dividers2.first),
            [&] (int id) { return id_mapping.at(id); });
    std::unordered_set<int> shrunk_groups;
    for (int i=0; i<child.vertex_group.size(); i++) {
        int parent1_group = parent1.vertex_group[i];
        if (id_mapping.count(parent1_group)) {
            shrunk_groups.insert(child.vertex_group[i]);
            child.vertex_group[i] = id_mapping.at(parent1_group);
        }
    }
    { // erase eventual empty groups
        const auto child_groups = util::groupIndexesByValue(child.vertex_group);
        for (auto it = shrunk_groups.begin(); it != shrunk_groups.end(); ) {
            if (!child_groups.count(*it)) {
                child.group_ids.erase(std::find(child.group_ids.begin(), child.group_ids.end(), *it));
                it = shrunk_groups.erase(it);
            } else {
                it++;
            }
        }
    }

    // phase 2 -- randomly move vertices from shrunk groups to the closest group
    {
        auto old_child_vertex_groups = child.vertex_group;
        auto child_groups = util::groupIndexesByValue(child.vertex_group);
        const int maximal_capacity = network_->edgeCapacity(network_->levelsNumber()-1);
        for (int g : shrunk_groups) {
            if (child_groups.at(g).size() >= move_k)
                continue;
            auto old_child_group = child_groups.at(g);
            for (int i : old_child_group) {
                if (probability_generator.generate() > move_p)
                    continue;
                int closest_v = closestTerminalInDifferentGroup(i, old_child_vertex_groups, *network_);
                int closest_v_group = child.vertex_group[closest_v];
                if (closest_v == network_->center() || closest_v_group == g) {
                    continue;
                }

                int group_load = std::accumulate(
                    child_groups.at(closest_v_group).begin(), child_groups.at(closest_v_group).end(), 0,
                    [&] (int acc, int v) {
                        return acc + network_->demand(v);
                    });
                if (group_load + network_->demand(i) <= maximal_capacity) {
                    child.vertex_group[i] = child.vertex_group[closest_v];
                }
            }
            if (!child.hasVertexWithId(g)) {
                util::erase(child.group_ids, g);
            }
        }
    }

    return child.refresh();
}

std::vector<internal::Chromosome> GeneticGamvros::crossoverChromosomes(const std::vector<internal::Chromosome>& parents)
{
    std::vector<internal::Chromosome> children;
    children.reserve(parents.size());
    for (int i=0; i+1 < parents.size(); i+=2) {
        const internal::Chromosome& parent1 = parents[i], &parent2 = parents[i+1];
        children.push_back(crossoverChromosomes(parent1, parent2));
        children.push_back(crossoverChromosomes(parent2, parent1));
    }
    return children;
}

std::vector<internal::Chromosome> GeneticGamvros::selectChromosomes(
        int N, const std::vector<std::pair<internal::Chromosome, double>> &population_with_fitness)
{
    const double sigma_truncation = params_.sigma_truncation;

    std::vector<double> chromosome_fitness;
    chromosome_fitness.reserve(params_.population_size);
    std::transform(
            population_with_fitness.begin(), population_with_fitness.end(),std::back_inserter(chromosome_fitness),
            [] (const auto& p) { return p.second; });
    double max_fitness = *std::max_element(chromosome_fitness.begin(), chromosome_fitness.end());
    double f_max = max_fitness + 1.0;
    double mean_fitness = util::mean(chromosome_fitness);
    double stdev_fitness = util::stdev(chromosome_fitness);

    auto modifiedFitness = [mean_fitness, stdev_fitness, sigma_truncation] (double f) {
        return f + mean_fitness - sigma_truncation * stdev_fitness;
    };
    double probability_divider = std::accumulate(chromosome_fitness.begin(), chromosome_fitness.end(), 0.0,
        [f_max, modifiedFitness] (double acc, double f) {
            double f_ = f_max - modifiedFitness(f);
            return acc + f_;
        });

    std::vector<double> probabilities;
    probabilities.reserve(params_.population_size);
    std::transform(chromosome_fitness.begin(), chromosome_fitness.end(), std::back_inserter(probabilities),
        [f_max, &modifiedFitness, probability_divider] (double f) {
            return (f_max - modifiedFitness(f)) / probability_divider;
        });

    util::number::NonUniformIntGenerator index_generator(0, probabilities);
    std::unordered_set<int> selected_indexes;
    while (selected_indexes.size() < N) {
        selected_indexes.insert(index_generator.generate());
    }
    std::vector<internal::Chromosome> selected_chromosomes;
    selected_chromosomes.reserve(N);
    std::transform(selected_indexes.begin(), selected_indexes.end(), std::back_inserter(selected_chromosomes),
        [&population_with_fitness] (int i) {
            return population_with_fitness[i].first;
        });
    return selected_chromosomes;
}

double GeneticGamvros::evaluateFitness(const internal::Chromosome& chromosome)
{
    auto mlcmst = subnet_solver_.solveMLCMST(*network_, chromosome.vertex_group);
    return mlcmst.cost(*network_);
}

std::vector<std::pair<internal::Chromosome, double> >
GeneticGamvros::evaluateFitness(const std::vector<internal::Chromosome> &chromosomes)
{
    std::vector< std::pair<internal::Chromosome, double> > chromosomes_with_fitness;
    chromosomes_with_fitness.reserve(chromosomes.size());
    std::transform(chromosomes.begin(), chromosomes.end(), std::back_inserter(chromosomes_with_fitness),
        [&] (const auto& chromosome) {
            return std::make_pair(chromosome, evaluateFitness(chromosome));
        });
    return chromosomes_with_fitness;
}

std::vector<internal::Chromosome> GeneticGamvros::selectMostFit(
        int N, const std::vector<std::pair<internal::Chromosome, double>> &population_with_fitness)
{
    auto chromosome_cmp =
        [] (const std::pair<internal::Chromosome, double>& p1, const std::pair<internal::Chromosome, double>& p2) {
            return p1.second > p2.second;
        };
    std::priority_queue<std::pair<internal::Chromosome, double>, std::vector<std::pair<internal::Chromosome, double>>,
        decltype(chromosome_cmp)> chromosomes_heap(
                population_with_fitness.begin(), population_with_fitness.end(), chromosome_cmp
    );

    std::vector<internal::Chromosome> most_fit_chromosomes;
    most_fit_chromosomes.reserve(N);
    while (N > 0 && !chromosomes_heap.empty()) {
        auto p = chromosomes_heap.top();
        chromosomes_heap.pop();
        most_fit_chromosomes.push_back(p.first);
        N--;
    }
    return most_fit_chromosomes;
}

internal::Chromosome GeneticGamvros::mutate(internal::Chromosome chromosome)
{
    using improvement::LinkUpgrade;
    using improvement::LocalSearch2006;
    LocalSearch2006 mutator(std::make_unique<LinkUpgrade>(LinkUpgrade::Params{
        true, true, false
    }), LocalSearch2006::Params { true });
    chromosome.vertex_group = mutator.improvementStep(*network_, chromosome.vertex_group);
    return internal::Chromosome(chromosome.center, chromosome.vertex_group).refresh();
}

std::vector<internal::Chromosome> GeneticGamvros::mutate(std::vector<internal::Chromosome> chromosomes)
{
    std::transform(chromosomes.begin(), chromosomes.end(), chromosomes.begin(),
            [&] (const internal::Chromosome& chromosome) { return mutate(chromosome); });
    return chromosomes;
}

int GeneticGamvros::closestTerminalInDifferentGroup(
        int v, const std::vector<int> &vertex_group, const network::MLCCNetwork &mlcc_network)
{
    auto distances = mlcc_network.network(0).costs()[v];
    auto v_group = util::groupIndexesByValue(vertex_group)[vertex_group[v]];
    for (int j : v_group) {
        distances[j] = std::numeric_limits<double>::max();
    }
    distances[mlcc_network.center()] = std::numeric_limits<double>::max();
    return std::distance(distances.begin(), std::min_element(distances.begin(), distances.end()));
}

}
