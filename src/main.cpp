#include <iostream>
#include <cmath>
#include <ctime>
#include <FacilityLocation/Objective.h>
#include <FacilityLocation/Solver.h>
#include "GA/Engine.h"
#include <GA/Crossover/MultiPointCrossover.h>
#include <GA/Selection/ProbabilistSelection.h>
#include <GA/Objective/DeadBitInsertion.h>
#include "GA/Mutation/RandomMutation.h"
#include "GA/Selection/ElitismSelection.h"

#define NF 128 // Number of facilities
#define NC NF // Number of customers

using realIndividual = GA::BinaryRepresentation<NF>;
using redundantIndividual = GA::BinaryRepresentation<NF + NF/2>;

int main(int argc, char **argv) {
    srand((unsigned int) time(nullptr));

    FacilityLocation::Instance<NF> instance = FacilityLocation::Instance<NF>::randomMetricInstance(NC, 1);

    FacilityLocation::Objective<realIndividual> realObjective(instance);
    GA::DeadBitInsertion<realIndividual, redundantIndividual> objective(realObjective, NF / 2);
    GA::MultiPointCrossover<redundantIndividual> crossover(1);
    GA::RandomMutation<redundantIndividual> mutation(1. / NF);
    GA::ProbabilistSelection<redundantIndividual> selection;

    GA::Engine<redundantIndividual> ga(objective, crossover, mutation, selection);

    if (NF * NC <= 16 * 16) {
        std::cout << "### Distances" << std::endl;
        for (size_t i = 0; i < NF; ++i) {
            for (size_t j = 0; j < NC; ++j) {
                std::cout << instance.distance(i, j) << "\t";
            }
            std::cout << std::endl;
        }
        std::cout << "### Costs" << std::endl;
        for (size_t i = 0; i < NF; ++i) {
            std::cout << instance.cost(i) << "\t";
        }
        std::cout << std::endl;
    }

    std::cout << "### Best score" << std::endl;
    if (NF <= 16) {
        std::cout << "(optimal) " << FacilityLocation::Solver<NF>::bruteForce(instance, realObjective) << std::endl;
    } else {
        std::cout << "(estimation) " << FacilityLocation::Solver<NF>::greedy(instance) << std::endl;
    }

    std::cout << "### Genetic algorithm" << std::endl;
    ga.initialize(10);
    double score = ga.getScore();
    for (int i = 0; i < 100; i++) {
        std::cout << "step " << i << ": " << ga.step(10) << "\t" << ga.getBest() << std::endl;
    }

    return 0;
}

