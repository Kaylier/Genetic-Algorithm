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
#define TIME_MAX 1. // maximal time of each execution in second
#define TIME_MAX_TOTAL 100. // maximal total time allocated for a single set of parameters

using realIndividual = GA::BinaryRepresentation<NF>;
using redundantIndividual = GA::BinaryRepresentation<NF*3>;

using Clock = std::chrono::high_resolution_clock;
using Duration = std::chrono::duration<double>;

template<class Ind>
void plot(std::ostream &os, GA::Engine<Ind> &ga) {
    constexpr size_t INITIAL_SIZE = 32;
    constexpr unsigned int STEPS = 10;

    auto start = Clock::now();
    auto end = start + Duration(TIME_MAX);

    ga.initialize(INITIAL_SIZE);
    os << Duration(Clock::now() - start).count() << " " << ga.getScore() << std::endl;
    while (Clock::now() < end) {
        os << Duration(Clock::now() - start).count() << " " << ga.step(STEPS) << std::endl;
    }
}

int main() {
    srand((unsigned int) time(nullptr));

    FacilityLocation::Instance<NF> instance = FacilityLocation::Instance<NF>::randomMetricInstance(NC, SEED);
    FacilityLocation::Objective<realIndividual> realObjective(instance);
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

//    FacilityLocation::Objective<realIndividual> objective(realObjective);
//    GA::DeadBitInsertion<realIndividual, redundantIndividual> objective(realObjective, NF / 2);
    GA::DuplicateBits<realIndividual, redundantIndividual> objective(realObjective, 2);
    GA::MultiPointCrossover<redundantIndividual> crossover(1);
    GA::RandomMutation<redundantIndividual> mutation(2. / NF);
    GA::ProbabilistSelection<redundantIndividual> selection;

    GA::Engine<redundantIndividual> ga(objective, crossover, mutation, selection);

    std::cout << "### Execution" << std::endl;
    std::ofstream file;

    constexpr unsigned int NUMBER_MAX = TIME_MAX_TOTAL / TIME_MAX;
    for (unsigned int number = 0; number < NUMBER_MAX; number++) {
        std::cout << (100 * number / NUMBER_MAX) << "% (" << number << "/" << NUMBER_MAX << ")\r" << std::flush;
        file.open("plot1_" + std::to_string(number));
        plot(file, ga);
        file.close();
    }

    return 0;
}

