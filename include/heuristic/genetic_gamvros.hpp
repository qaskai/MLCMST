#pragma once

#include <vector>
#include <memory>

#include <mlcmst_solver.hpp>
#include <mlcmst_subnet_solver.hpp>

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
    [[nodiscard]] Chromosome refreshIds() const;
};
}

class GeneticGamvros final : public MLCMSTSolver
{
public:
    static std::string id();

    struct Params {
        int population_size = 100;
        int most_fit_mutate_number = 10;
        int parents_number = 70;
        int generations_number = 10;

        double network_fuzzing_epsilon = 0.5;

        double crossover_shrunk_move_probability = 0.9;
        int crossover_move_less_than_k = 6;
    };

    GeneticGamvros(
            std::vector<std::unique_ptr< MLCMSTSolver > > init_population_solvers,
            std::unique_ptr< MLCMSTSolver > subnet_solver,
            const Params& params);
    ~GeneticGamvros() override;

    Result solve(const network::MLCCNetwork &mlcc_network) override;

private:
    static double EPS_;

    const network::MLCCNetwork* network_;

    Params params_;
    std::vector<std::unique_ptr< MLCMSTSolver >> init_population_solvers_;
    MLCMST_SubnetSolver subnet_solver_;

    std::vector<internal::Chromosome> initializePopulation();

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
