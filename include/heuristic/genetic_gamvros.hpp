#pragma once

#include <vector>
#include <memory>

#include <mlcmst_solver.hpp>
#include <mlcmst_subnet_solver.hpp>
#include <heuristic/mlcmst_heuristic.hpp>

#include <json/property.hpp>

#include <util/number/int_generator.hpp>

namespace MLCMST::heuristic {

namespace internal {
struct Chromosome
{
    int center;
    std::vector<int> vertex_group;
    std::vector<int> group_ids;
    Chromosome(int center, const std::vector<int>& vertex_group);

public:
    static std::vector<int> createGroupIdVector(int center, const std::vector<int>& vertex_group);
    [[nodiscard]] int freeId() const;
    [[nodiscard]] Chromosome refreshIds() const;
};
bool operator==(const Chromosome& c1, const Chromosome& c2);
bool operator!=(const Chromosome& c1, const Chromosome& c2);
}

// TODO: Falls into local minima very fast. Experiment with it.
class GeneticGamvros final : public MLCMST_Heuristic
{
public:
    static std::string id();

    struct Params {
        int population_size = 100;
        int most_fit_mutate_number = 10;
        int parents_number = 70;
        int generations_number = 10;

        double sigma_truncation = 3.0;
        double network_fuzzing_epsilon = 0.5;
        double crossover_shrunk_move_probability = 0.9;

        int crossover_move_less_than_k = 6;

        constexpr static auto properties = std::make_tuple(
            json::Property<Params, int>{&Params::population_size, "population_size"},
            json::Property<Params, int>{&Params::most_fit_mutate_number, "most_fit_mutate_number"},
            json::Property<Params, int>{&Params::parents_number, "parents_number"},
            json::Property<Params, int>{&Params::generations_number, "generations_number"},
            json::Property<Params, double>{&Params::network_fuzzing_epsilon, "network_fuzzing_epsilon"},
            json::Property<Params, double>{&Params::sigma_truncation, "sigma_truncation"},
            json::Property<Params, double>{&Params::crossover_shrunk_move_probability, "crossover_shrunk_move_probability"},
            json::Property<Params, int>{&Params::crossover_move_less_than_k, "crossover_move_less_than_k"}
            );
    };

    GeneticGamvros(
            std::vector<std::unique_ptr< MLCMST_Solver > > init_population_solvers,
            std::unique_ptr< MLCMST_Solver > subnet_solver,
            const Params& params);
    ~GeneticGamvros() override;

    network::MLCST run(const network::MLCCNetwork &mlcc_network) override;

private:
    static const double EPS_;

    const network::MLCCNetwork* network_;

    Params params_;
    std::vector<std::unique_ptr< MLCMST_Solver >> init_population_solvers_;
    MLCMST_SubnetSolver subnet_solver_;

    std::vector<internal::Chromosome> initializePopulation();
    std::vector< internal::Chromosome > forceDiversity(std::vector<internal::Chromosome> population);
    internal::Chromosome diversify(internal::Chromosome c);

    double evaluateFitness(const internal::Chromosome& chromosome);
    std::vector< std::pair< internal::Chromosome, double > > evaluateFitness(
            const std::vector< internal::Chromosome >& chromosomes);

    std::vector<internal::Chromosome> selectChromosomes(
            int N, const std::vector< std::pair<internal::Chromosome, double> >& population_with_fitness);

    internal::Chromosome crossoverChromosomes(const internal::Chromosome& parent1, const internal::Chromosome& parent2);
    std::vector< internal::Chromosome > crossoverChromosomes(const std::vector< internal::Chromosome >& parents);

    std::vector<internal::Chromosome> selectMostFit(
            int N, const std::vector< std::pair<internal::Chromosome, double> >& population_with_fitness);
    internal::Chromosome mutate(internal::Chromosome chromosome);
    std::vector< internal::Chromosome > mutate(std::vector< internal::Chromosome > chromosomes);
};

}
