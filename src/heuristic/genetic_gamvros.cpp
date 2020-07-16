#include <heuristic/genetic_gamvros.hpp>

#include <chrono>
#include <unordered_map>
#include <unordered_set>
#include <queue>

#include <heuristic/local_search_2006.hpp>

#include <util/number/int_generator.hpp>
#include <util/number/real_number_generator.hpp>
#include <util/number/non_uniform_int_generator.hpp>
#include <util/util.hpp>

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

Chromosome Chromosome::refreshIds() const
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

}



GeneticGamvros::GeneticGamvros(std::vector<std::unique_ptr<MLCMSTSolver>> init_population_solvers,
        std::unique_ptr<MLCMSTSolver> subnet_solver, const Params& params)
    : params_(params), init_population_solvers_(std::move(init_population_solvers)),
      subnet_solver_(std::move(subnet_solver))
{
}

GeneticGamvros::~GeneticGamvros() = default;

double GeneticGamvros::EPS_ = 1e-6;

MLCMSTSolver::Result GeneticGamvros::solve(const network::MLCCNetwork &mlcc_network)
{
    network_ = &mlcc_network;

    auto time_start = std::chrono::high_resolution_clock::now();

    std::vector< internal::Chromosome > population = initializePopulation();
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
        new_population.insert(new_population.end(), children.begin(), children.end());
        population = new_population;
    }
    internal::Chromosome most_fit_chromosome = selectChromosomes(1, evaluateFitness(population))[0];

    auto time_end = std::chrono::high_resolution_clock::now();
    double wall_time = std::chrono::duration<double, std::milli>(time_end - time_start).count();

    return Result{
        subnet_solver_.solveMLCMST(*network_, most_fit_chromosome.group_ids),
        std::nullopt,
        wall_time,
        true
    };
}

std::vector<internal::Chromosome> GeneticGamvros::initializePopulation()
{
    util::number::RealNumberGenerator epsilon_generator(
            1.0 - params_.network_fuzzing_epsilon, 1.0 + params_.network_fuzzing_epsilon);
    std::vector<internal::Chromosome> population;
    population.reserve(params_.population_size);

    while (population.size() < params_.population_size) {
        for (int i=0; i < init_population_solvers_.size() && population.size() < params_.population_size; i++){
            network::MLCMST mlcmst = init_population_solvers_[i]->solve(
                    network_->multiplyEdgeCosts(epsilon_generator.generate())).mlcmst.value();
            population.emplace_back(network_->center(), mlcmst.subnet());
        }
    }
    return population;
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

    // phase 1 -- move override grouping int parent2 with selected groups of parent1
    std::unordered_map<int, int> id_mapping;
    {
        std::vector<int> moved_groups(
                parent1.group_ids.begin() + dividers1.first, parent1.group_ids.begin() + dividers1.second);
        std::vector<int> new_ids = getFirstNFreeIds(moved_groups.size(), parent2.vertex_group);
        for (int i = 0; i < new_ids.size(); i++) {
            id_mapping[moved_groups[i]] = new_ids[i];
        }
    }
    std::transform(parent1.group_ids.begin() + dividers1.first, parent1.group_ids.begin() + dividers1.second,
            std::inserter(child.group_ids, child.group_ids.begin() + dividers2.first),
            [&] (int id) { return id_mapping.at(id); });
    std::unordered_set<int> shrunk_groups;
    for (int i=0; i<child.vertex_group.size(); i++) {
        int parent1_group = parent1.vertex_group[i];
        if (id_mapping.count(parent1_group)) {
            shrunk_groups.insert(child.vertex_group[i]);
            child.vertex_group[i] = id_mapping[parent1_group];
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
        const auto child_groups = util::groupIndexesByValue(child.vertex_group);
        for (int g : shrunk_groups) {
            int count = child_groups.at(g).size();
            if (count >= move_k)
                continue;
            for (int i : child_groups.at(g)) {
                if (probability_generator.generate() > move_p)
                    continue;
                auto distances = network_->network(0).costs()[i];
                distances[network_->center()] = std::numeric_limits<double>::max();
                for (int j : child_groups.at(g)) {
                    distances[j] = std::numeric_limits<double>::max();
                }
                int closest_vertex_in_different_group =
                        std::distance(distances.begin(), std::min_element(distances.begin(), distances.end()));
                child.vertex_group[i] = child.vertex_group[closest_vertex_in_different_group];
                count--;
            }
            if (count == 0) {
                child.group_ids.erase(std::find(child.group_ids.begin(), child.group_ids.end(), g));
            }
        }
    }

    return child.refreshIds();
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
    const double stdev_multiplier_constant = 3;

    std::vector<double> chromosome_fitness;
    chromosome_fitness.reserve(params_.population_size);
    std::transform(
            population_with_fitness.begin(), population_with_fitness.end(),std::back_inserter(chromosome_fitness),
            [] (const auto& p) { return p.second; });
    double max_fitness = *std::max_element(chromosome_fitness.begin(), chromosome_fitness.end());
    double mean_fitness = util::mean(chromosome_fitness);
    double stdev_fitness = util::stdev(chromosome_fitness);

    auto modifiedFitness = [mean_fitness, stdev_fitness, stdev_multiplier_constant] (double f) {
        return f + mean_fitness - stdev_multiplier_constant * stdev_fitness;
    };
    double probability_divider = std::accumulate(chromosome_fitness.begin(), chromosome_fitness.end(), 0.0,
        [max_fitness, modifiedFitness] (double acc, double f) {
            double f_ = max_fitness - modifiedFitness(f);
            return acc + f_;
        });

    std::vector<double> probabilities;
    probabilities.reserve(params_.population_size);
    std::transform(chromosome_fitness.begin(), chromosome_fitness.end(), std::back_inserter(probabilities),
        [max_fitness, modifiedFitness, probability_divider] (double f) {
            return (max_fitness - modifiedFitness(f)) / probability_divider;
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
    LocalSearch2006 mutator;
    chromosome.vertex_group = mutator.improvementStep(*network_, chromosome.vertex_group);
    chromosome.group_ids = internal::Chromosome::createGroupIdVector(chromosome.center, chromosome.vertex_group);
    return chromosome;
}

std::vector<internal::Chromosome> GeneticGamvros::mutate(std::vector<internal::Chromosome> chromosomes)
{
    std::transform(chromosomes.begin(), chromosomes.end(), chromosomes.begin(),
            [&] (const internal::Chromosome& chromosome) { return mutate(chromosome); });
    return chromosomes;
}

}
