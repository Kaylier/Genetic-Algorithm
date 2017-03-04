#include <iostream>
#include <fstream>
#include <cmath>
#include <chrono>
#include "FacilityLocation/Objective.h"
#include "FacilityLocation/Solver.h"
#include "GA/Engine.h"
#include "GA/Crossover/MultiPointCrossover.h"
#include "GA/Selection/ProbabilistSelection.h"
#include "GA/Objective/DeadBitInsertion.h"
#include "GA/Objective/DuplicateBits.h"
#include "GA/Mutation/RandomMutation.h"
#include "GA/Selection/ElitismSelection.h"

#define NF 128 // Number of facilities
#define NC 128 // Number of customers
#define SEED 1 // Seed of the generation
#define TIME_MAX_EACH 1. // Time of each execution in seconds
#define TIME_MAX_TOTAL 100. // Time to spend with each set of parameters in seconds

using Individual = GA::BinaryRepresentation<NF>;

using Clock = std::chrono::high_resolution_clock;
using Duration = std::chrono::duration<double>;

template<class Individual>
void generate(double time_max_each, double time_max_total, std::string path, size_t initial_size,
              GA::Objective<Individual> &objective,
              GA::Crossover<Individual> &crossover,
              GA::Mutation<Individual> &mutation,
              GA::Selection<Individual> &selection) {
    constexpr unsigned int STEPS = 10;

    GA::Engine<Individual> ga(objective, crossover, mutation, selection);
    std::cout << "### Execution (" << path << ")" << std::endl;
    std::ofstream file;

    unsigned int number_max = time_max_total / time_max_each;
    for (unsigned int number = 0; number < number_max; number++) {
        std::cout << (100 * number / number_max) << "% (" << number << "/" << number_max << ")\r" << std::flush;
        file.open(path + std::to_string(number));
        if (!file.is_open()) {
            std::cerr << "Can't open file " + path + std::to_string(number) << std::endl;
        } else {

            auto start = Clock::now();
            auto end = start + Duration(time_max_each);

            ga.initialize(initial_size);
            file << Duration(Clock::now() - start).count() << " " << ga.getScore() << std::endl;
            while (Clock::now() < end) {
                file << Duration(Clock::now() - start).count() << " " << ga.step(STEPS) << std::endl;
            }

            file.close();
        }
    }
}

int main() {
    srand((unsigned int) time(nullptr));

    FacilityLocation::Instance<NF> instance = FacilityLocation::Instance<NF>::randomMetricInstance(NC, SEED);
    FacilityLocation::Objective<Individual> realObjective(instance);
//    instance.save("facility_instance_" + std::to_string(SEED));
//    instance = FacilityLocation::Instance<NF>::load("facility_instance_" + std::to_string(SEED));

    std::cout << "### Problem description" << std::endl;
    std::cout << "Number of facility: " << NF << std::endl;
    std::cout << "Number of customer: " << NC << std::endl;
    std::cout << "Seed: " << SEED << std::endl;

    if (NF <= 16) {
        std::cout << "Best score: " << FacilityLocation::Solver<NF>::bruteForce(instance, realObjective) << std::endl;
    } else {
        std::cout << "Best score estimated (< 1.61*opt): " << FacilityLocation::Solver<NF>::greedy(instance) << std::endl;
    }

    {
        GA::MultiPointCrossover<Individual> crossover(1);
        GA::RandomMutation<Individual> mutation(1. / NF);
        GA::ProbabilistSelection<Individual> selection;
        generate(TIME_MAX_EACH, TIME_MAX_TOTAL, "output/plot0/", 32, realObjective, crossover, mutation, selection);
    }
    {
        using redundantIndividual = GA::BinaryRepresentation<NF*3>;
        GA::DuplicateBits<Individual, redundantIndividual> objective(realObjective, 0);
        GA::MultiPointCrossover<redundantIndividual> crossover(1);
        GA::RandomMutation<redundantIndividual> mutation(1. / NF);
        GA::ProbabilistSelection<redundantIndividual> selection;
        generate(TIME_MAX_EACH, TIME_MAX_TOTAL, "output/plot1/", 32, objective, crossover, mutation, selection);
    }
    return 0;
}

