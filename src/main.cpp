#include <iostream>
#include <cmath>
#include <ctime>
#include <FacilityLocation/Objective.h>
#include <FacilityLocation/Solver.h>
#include "GA/Engine.h"
#include <GA/Crossover/SinglePointCrossover.h>
#include "GA/Mutation/RandomMutation.h"
#include "GA/Selection/ElitismSelection.h"

#define NF 4 // Number of facilities
#define NC 4 // Number of customers

using Individual = GA::BinaryRepresentation<NF>;

int main(int argc, char **argv) {
    srand((unsigned int) time(nullptr));
    
    FacilityLocation::Instance<NF> instance = FacilityLocation::Instance<NF>::randomMetricInstance(NC);

    FacilityLocation::Objective<Individual> objective(instance);
    GA::SinglePointCrossover<Individual> crossover;
    GA::RandomMutation<Individual> mutation(1./NF);
    GA::ElitismSelection<Individual> selection(0.5);

    GA::Engine<Individual> ga(objective, crossover, mutation, selection);

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
/*
    std::cout << "### Scores" << std::endl;
    double bestScore = INFINITY;
    for (int n = 0; n < 32; ++n) {
        Individual precomputed(n);
        double score = objective(precomputed);
        std::cout << precomputed << ": " << score << std::endl;
        if (score < bestScore) {
            bestScore = score;
        }
    }
*/
    std::cout << "### Best score" << std::endl;
    std::cout << FacilityLocation::Solver<NF>::bruteForce(instance, objective) << std::endl;

    ga.initialize(10);
    for (int i = 0; i < 10; i++) {
        std::cout << "step " << i << ": " << ga.step() << "\t" << ga.getBest() << std::endl;
    }
/*
    for (int i = 10; i < 101; i+=10) {
        std::cout << "step " << i << ": " << ga.step(10) << std::endl;
    }
*/

    return 0;
}

