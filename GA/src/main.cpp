#include <iostream>
#include <fstream>
#include <cmath>
#include <chrono>
#include <string>
#include <GA/Crossover/SinglePointCrossover.h>
#include "FacilityLocation/Objective.h"
#include "FacilityLocation/Solver.h"
#include "GA/Engine.h"
#include "GA/Crossover/MultiPointCrossover.h"
#include "GA/Selection/ElitismSelection.h"
#include "GA/Objective/DeadBitInsertion.h"
#include "GA/Objective/DuplicateBits.h"
#include "GA/Objective/MixInformation.h"
#include "GA/Mutation/RandomMutation.h"

#define NF 100 // Number of facilities
#define NC 1000 // Number of customers
#define SEED 1 // Seed of the generation
#define ORDERED true
#define TIME_MAX_EACH 1.0 // Time of each execution in seconds
#define TIME_MAX_TOTAL TIME_MAX_EACH*100 // Time to spend with each set of parameters in seconds

#define PATH "output/" + std::to_string(NF) + "-" + std::to_string(NC) + "-" + std::to_string(SEED) + (ORDERED?"-ordered/":"/")
#define MKDIR std::string("mkdir -p ")

using Individual = GA::BinaryRepresentation<NF>;

using Clock = std::chrono::high_resolution_clock;
using Duration = std::chrono::duration<double>;

template<class Individual>
void generate(double time_max_each, double time_max_total, std::string path, size_t initial_size,
              GA::Objective<Individual> &objective,
              GA::Crossover<Individual> &crossover,
              GA::Mutation<Individual> &mutation,
              GA::Selection<Individual> &selection) {
    constexpr unsigned int STEPS = 1;

    GA::Engine<Individual> ga(objective, crossover, mutation, selection);
    std::cout << "### Execution of " << path << std::endl;
    std::ofstream file;

    unsigned int number_max = time_max_total / time_max_each;
    for (unsigned int number = 0; number < number_max; number++) {
        std::cout << (100 * number / number_max) << "% (" << number << "/" << number_max << ")\r" << std::flush;
        file.open(path + "/" + std::to_string(number));
        if (!file.is_open()) {
            std::cerr << "Can't open file " + path + "/" + std::to_string(number) << std::endl;
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

    FacilityLocation::Instance<NF> instance = FacilityLocation::Instance<NF>::randomFlawedMetricInstance(NC, SEED, ORDERED);
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
        auto start = Clock::now();
        std::cout << "Best score estimated (< 1.61*opt): " << FacilityLocation::Solver<NF>::greedy(instance);
        std::cout << " (computed in " << Duration(Clock::now() - start).count() << "s)" << std::endl;
    }

    // No redundancy reference execution
    {
        GA::SinglePointCrossover<Individual> crossover;
        GA::RandomMutation<Individual> mutation(1. / NF);
        GA::ElitismSelection<Individual> selection(0.05);
        system((MKDIR+PATH+"no-redundancy/0/").c_str());
        generate(TIME_MAX_EACH, TIME_MAX_TOTAL, PATH + "no-redundancy/0/", 128, realObjective,
                 crossover, mutation, selection);
    }

    // Benchmark of the mutation rate
    for (double x: {0., 0.1, 0.2, 0.5, 1.0, 1.5, 2.0, 3.0, 5.0}) {
        GA::MultiPointCrossover<Individual> crossover(1);
        GA::RandomMutation<Individual> mutation(x / NF);
        GA::ElitismSelection<Individual> selection(0.05);
        system((MKDIR+PATH+"bench_random_mutation/"+std::to_string(x)).c_str());
        generate(TIME_MAX_EACH, TIME_MAX_TOTAL, PATH + "/bench_random_mutation/"+std::to_string(x), 128,
                 realObjective, crossover, mutation, selection);
    }

    // Benchmark of the selection rate
/*
    for (double x: {0., 0.01, 0.02, 0.05, 0.10, 0.20, 0.50}) {
        GA::MultiPointCrossover<Individual> crossover(1);
        GA::RandomMutation<Individual> mutation(1. / NF);
        GA::ElitismSelection<Individual> selection(x);
        system((MKDIR+PATH+"bench_elitism_selection/"+std::to_string(x)).c_str());
        generate(TIME_MAX_EACH, TIME_MAX_TOTAL, PATH + "/bench_elitism_selection/"+std::to_string(x), 128,
                 realObjective, crossover, mutation, selection);
    }
*/

    // Benchmark of the population size
/*
    for (size_t n: {2, 4, 8, 16, 32, 64, 96, 128, 160, 192}) {
        GA::MultiPointCrossover<Individual> crossover(1);
        GA::RandomMutation<Individual> mutation(1. / NF);
        GA::ElitismSelection<Individual> selection(0.05);
        system((MKDIR+PATH+"bench_population_size/"+std::to_string(n)).c_str());
        generate(TIME_MAX_EACH, TIME_MAX_TOTAL, PATH + "bench_population_size/"+std::to_string(n), n,
                 realObjective, crossover, mutation, selection);
    }
*/

    // Benchmark of the crossover number of cut
/*
    for (size_t n: {1, 2, 3, 4}) {
        GA::MultiPointCrossover<Individual> crossover(n);
        GA::RandomMutation<Individual> mutation(1. / NF);
        GA::ElitismSelection<Individual> selection(0.05);
        system((MKDIR+PATH+"bench_crossover_point/"+std::to_string(n)).c_str());
        generate(TIME_MAX_EACH, TIME_MAX_TOTAL, PATH + "/bench_crossover_point/"+std::to_string(n), 128,
                 realObjective, crossover, mutation, selection);
    }
*/

    // Test of dead bits
    {
        {
            constexpr size_t N = NF;
            using redundantIndividual = GA::BinaryRepresentation<N>;
            GA::DeadBitInsertion<Individual, redundantIndividual> objective(realObjective, 0);
            GA::SinglePointCrossover<redundantIndividual> crossover;
            GA::RandomMutation<redundantIndividual> mutation(1. / NF);
            GA::ElitismSelection<redundantIndividual> selection(0.05);
            system((MKDIR+PATH+"dead-bit-beg/"+std::to_string(N)).c_str());
            generate(TIME_MAX_EACH, TIME_MAX_TOTAL, PATH + "dead-bit-beg/" + std::to_string(N), 128, objective,
                     crossover, mutation, selection);
        }
        {
            constexpr size_t N = NF;
            using redundantIndividual = GA::BinaryRepresentation<N>;
            GA::DeadBitInsertion<Individual, redundantIndividual> objective(realObjective, NF/2);
            GA::SinglePointCrossover<redundantIndividual> crossover;
            GA::RandomMutation<redundantIndividual> mutation(1. / NF);
            GA::ElitismSelection<redundantIndividual> selection(0.05);
            system((MKDIR+PATH+"dead-bit-mid/"+std::to_string(N)).c_str());
            generate(TIME_MAX_EACH, TIME_MAX_TOTAL, PATH + "dead-bit-mid/" + std::to_string(N), 128, objective,
                     crossover, mutation, selection);
        }
        {
            constexpr size_t N = NF*3/2;
            using redundantIndividual = GA::BinaryRepresentation<N>;
            GA::DeadBitInsertion<Individual, redundantIndividual> objective(realObjective, 0);
            GA::SinglePointCrossover<redundantIndividual> crossover;
            GA::RandomMutation<redundantIndividual> mutation(1. / NF);
            GA::ElitismSelection<redundantIndividual> selection(0.05);
            system((MKDIR+PATH+"dead-bit-beg/"+std::to_string(N)).c_str());
            generate(TIME_MAX_EACH, TIME_MAX_TOTAL, PATH + "dead-bit-beg/" + std::to_string(N), 128, objective,
                     crossover, mutation, selection);
        }
        {
            constexpr size_t N = NF*3/2;
            using redundantIndividual = GA::BinaryRepresentation<N>;
            GA::DeadBitInsertion<Individual, redundantIndividual> objective(realObjective, NF/2);
            GA::SinglePointCrossover<redundantIndividual> crossover;
            GA::RandomMutation<redundantIndividual> mutation(1. / NF);
            GA::ElitismSelection<redundantIndividual> selection(0.05);
            system((MKDIR+PATH+"dead-bit-mid/"+std::to_string(N)).c_str());
            generate(TIME_MAX_EACH, TIME_MAX_TOTAL, PATH + "dead-bit-mid/" + std::to_string(N), 128, objective,
                     crossover, mutation, selection);
        }
        {
            constexpr size_t N = NF*2;
            using redundantIndividual = GA::BinaryRepresentation<N>;
            GA::DeadBitInsertion<Individual, redundantIndividual> objective(realObjective, 0);
            GA::SinglePointCrossover<redundantIndividual> crossover;
            GA::RandomMutation<redundantIndividual> mutation(1. / NF);
            GA::ElitismSelection<redundantIndividual> selection(0.05);
            system((MKDIR+PATH+"dead-bit-beg/"+std::to_string(N)).c_str());
            generate(TIME_MAX_EACH, TIME_MAX_TOTAL, PATH + "dead-bit-beg/" + std::to_string(N), 128, objective,
                     crossover, mutation, selection);
        }
        {
            constexpr size_t N = NF*2;
            using redundantIndividual = GA::BinaryRepresentation<N>;
            GA::DeadBitInsertion<Individual, redundantIndividual> objective(realObjective, NF/2);
            GA::SinglePointCrossover<redundantIndividual> crossover;
            GA::RandomMutation<redundantIndividual> mutation(1. / NF);
            GA::ElitismSelection<redundantIndividual> selection(0.05);
            system((MKDIR+PATH+"dead-bit-mid/"+std::to_string(N)).c_str());
            generate(TIME_MAX_EACH, TIME_MAX_TOTAL, PATH + "dead-bit-mid/" + std::to_string(N), 128, objective,
                     crossover, mutation, selection);
        }
        {
            constexpr size_t N = NF*3;
            using redundantIndividual = GA::BinaryRepresentation<N>;
            GA::DeadBitInsertion<Individual, redundantIndividual> objective(realObjective, 0);
            GA::SinglePointCrossover<redundantIndividual> crossover;
            GA::RandomMutation<redundantIndividual> mutation(1. / NF);
            GA::ElitismSelection<redundantIndividual> selection(0.05);
            system((MKDIR+PATH+"duplicate-bit/"+std::to_string(N)).c_str());
            system((MKDIR+PATH+"dead-bit-beg/"+std::to_string(N)).c_str());
            generate(TIME_MAX_EACH, TIME_MAX_TOTAL, PATH + "dead-bit-beg/" + std::to_string(N), 128, objective,
                     crossover, mutation, selection);
        }
        {
            constexpr size_t N = NF*3;
            using redundantIndividual = GA::BinaryRepresentation<N>;
            GA::DeadBitInsertion<Individual, redundantIndividual> objective(realObjective, NF/2);
            GA::SinglePointCrossover<redundantIndividual> crossover;
            GA::RandomMutation<redundantIndividual> mutation(1. / NF);
            GA::ElitismSelection<redundantIndividual> selection(0.05);
            system((MKDIR+PATH+"dead-bit-mid/"+std::to_string(N)).c_str());
            generate(TIME_MAX_EACH, TIME_MAX_TOTAL, PATH + "dead-bit-mid/" + std::to_string(N), 128, objective,
                     crossover, mutation, selection);
        }
    }
    return 0;

    // Test of bit duplication
    {
        {
            constexpr size_t N = NF;
            using redundantIndividual = GA::BinaryRepresentation<N>;
            GA::DuplicateBits<Individual, redundantIndividual> objective(realObjective, 0);
            GA::SinglePointCrossover<redundantIndividual> crossover;
            GA::RandomMutation<redundantIndividual> mutation(1. / NF);
            GA::ElitismSelection<redundantIndividual> selection(0.05);
            system((MKDIR+PATH+"duplicate-bit/"+std::to_string(N)+"_0").c_str());
            generate(TIME_MAX_EACH, TIME_MAX_TOTAL, PATH + "duplicate-bit/" + std::to_string(N) + "_0", 128, objective,
                     crossover, mutation, selection);
        }
        {
            constexpr size_t N = NF;
            using redundantIndividual = GA::BinaryRepresentation<N>;
            GA::DuplicateBits<Individual, redundantIndividual> objective(realObjective, 1);
            GA::SinglePointCrossover<redundantIndividual> crossover;
            GA::RandomMutation<redundantIndividual> mutation(1. / NF);
            GA::ElitismSelection<redundantIndividual> selection(0.05);
            system((MKDIR+PATH+"duplicate-bit/"+std::to_string(N)+"_1").c_str());
            generate(TIME_MAX_EACH, TIME_MAX_TOTAL, PATH + "duplicate-bit/" + std::to_string(N) + "_1", 128, objective,
                     crossover, mutation, selection);
        }
        {
            constexpr size_t N = NF;
            using redundantIndividual = GA::BinaryRepresentation<N>;
            GA::DuplicateBits<Individual, redundantIndividual> objective(realObjective, 2);
            GA::SinglePointCrossover<redundantIndividual> crossover;
            GA::RandomMutation<redundantIndividual> mutation(1. / NF);
            GA::ElitismSelection<redundantIndividual> selection(0.05);
            system((MKDIR+PATH+"duplicate-bit/"+std::to_string(N)+"_2").c_str());
            generate(TIME_MAX_EACH, TIME_MAX_TOTAL, PATH + "duplicate-bit/" + std::to_string(N) + "_2", 128, objective,
                     crossover, mutation, selection);
        }
        {
            constexpr size_t N = NF * 3;
            using redundantIndividual = GA::BinaryRepresentation<N>;
            GA::DuplicateBits<Individual, redundantIndividual> objective(realObjective, 0);
            GA::SinglePointCrossover<redundantIndividual> crossover;
            GA::RandomMutation<redundantIndividual> mutation(1. / N);
            GA::ElitismSelection<redundantIndividual> selection(0.05);
            system((MKDIR+PATH+"duplicate-bit/"+std::to_string(N)+"_0").c_str());
            generate(TIME_MAX_EACH, TIME_MAX_TOTAL, PATH + "duplicate-bit/" + std::to_string(N) + "_0", 128, objective,
                     crossover, mutation, selection);
        }
        {
            constexpr size_t N = NF * 3;
            using redundantIndividual = GA::BinaryRepresentation<N>;
            GA::DuplicateBits<Individual, redundantIndividual> objective(realObjective, 1);
            GA::SinglePointCrossover<redundantIndividual> crossover;
            GA::RandomMutation<redundantIndividual> mutation(1. / N);
            GA::ElitismSelection<redundantIndividual> selection(0.05);
            system((MKDIR+PATH+"duplicate-bit/"+std::to_string(N)+"_1").c_str());
            generate(TIME_MAX_EACH, TIME_MAX_TOTAL, PATH + "duplicate-bit/" + std::to_string(N) + "_1", 129, objective,
                     crossover, mutation, selection);
        }
        {
            constexpr size_t N = NF * 3;
            using redundantIndividual = GA::BinaryRepresentation<N>;
            GA::DuplicateBits<Individual, redundantIndividual> objective(realObjective, 2);
            GA::SinglePointCrossover<redundantIndividual> crossover;
            GA::RandomMutation<redundantIndividual> mutation(1. / N);
            GA::ElitismSelection<redundantIndividual> selection(0.05);
            system((MKDIR+PATH+"duplicate-bit/"+std::to_string(N)+"_2").c_str());
            generate(TIME_MAX_EACH, TIME_MAX_TOTAL, PATH + "duplicate-bit/" + std::to_string(N) + "_2", 128, objective,
                     crossover, mutation, selection);
        }
        {
            constexpr size_t N = NF * 5;
            using redundantIndividual = GA::BinaryRepresentation<N>;
            GA::DuplicateBits<Individual, redundantIndividual> objective(realObjective, 0);
            GA::SinglePointCrossover<redundantIndividual> crossover;
            GA::RandomMutation<redundantIndividual> mutation(1. / N);
            GA::ElitismSelection<redundantIndividual> selection(0.05);
            system((MKDIR+PATH+"duplicate-bit/"+std::to_string(N)+"_0").c_str());
            generate(TIME_MAX_EACH, TIME_MAX_TOTAL, PATH + "duplicate-bit/" + std::to_string(N) + "_0", 128, objective,
                     crossover, mutation, selection);
        }
        {
            constexpr size_t N = NF * 5;
            using redundantIndividual = GA::BinaryRepresentation<N>;
            GA::DuplicateBits<Individual, redundantIndividual> objective(realObjective, 1);
            GA::SinglePointCrossover<redundantIndividual> crossover;
            GA::RandomMutation<redundantIndividual> mutation(1. / N);
            GA::ElitismSelection<redundantIndividual> selection(0.05);
            system((MKDIR+PATH+"duplicate-bit/"+std::to_string(N)+"_1").c_str());
            generate(TIME_MAX_EACH, TIME_MAX_TOTAL, PATH + "duplicate-bit/" + std::to_string(N) + "_1", 128, objective,
                     crossover, mutation, selection);
        }
        {
            constexpr size_t N = NF * 5;
            using redundantIndividual = GA::BinaryRepresentation<N>;
            GA::DuplicateBits<Individual, redundantIndividual> objective(realObjective, 2);
            GA::SinglePointCrossover<redundantIndividual> crossover;
            GA::RandomMutation<redundantIndividual> mutation(1. / N);
            GA::ElitismSelection<redundantIndividual> selection(0.05);
            system((MKDIR+PATH+"duplicate-bit/"+std::to_string(N)+"_2").c_str());
            generate(TIME_MAX_EACH, TIME_MAX_TOTAL, PATH + "duplicate-bit/" + std::to_string(N) + "_2", 128, objective,
                     crossover, mutation, selection);
        }
        {
            constexpr size_t N = NF * 7;
            using redundantIndividual = GA::BinaryRepresentation<N>;
            GA::DuplicateBits<Individual, redundantIndividual> objective(realObjective, 0);
            GA::SinglePointCrossover<redundantIndividual> crossover;
            GA::RandomMutation<redundantIndividual> mutation(1. / N);
            GA::ElitismSelection<redundantIndividual> selection(0.05);
            system((MKDIR+PATH+"duplicate-bit/"+std::to_string(N)+"_0").c_str());
            generate(TIME_MAX_EACH, TIME_MAX_TOTAL, PATH + "duplicate-bit/" + std::to_string(N) + "_0", 128, objective,
                     crossover, mutation, selection);
        }
        {
            constexpr size_t N = NF * 7;
            using redundantIndividual = GA::BinaryRepresentation<N>;
            GA::DuplicateBits<Individual, redundantIndividual> objective(realObjective, 1);
            GA::SinglePointCrossover<redundantIndividual> crossover;
            GA::RandomMutation<redundantIndividual> mutation(1. / N);
            GA::ElitismSelection<redundantIndividual> selection(0.05);
            system((MKDIR+PATH+"duplicate-bit/"+std::to_string(N)+"_1").c_str());
            generate(TIME_MAX_EACH, TIME_MAX_TOTAL, PATH + "duplicate-bit/" + std::to_string(N) + "_1", 128, objective,
                     crossover, mutation, selection);
        }
        {
            constexpr size_t N = NF * 7;
            using redundantIndividual = GA::BinaryRepresentation<N>;
            GA::DuplicateBits<Individual, redundantIndividual> objective(realObjective, 2);
            GA::SinglePointCrossover<redundantIndividual> crossover;
            GA::RandomMutation<redundantIndividual> mutation(1. / N);
            GA::ElitismSelection<redundantIndividual> selection(0.05);
            system((MKDIR+PATH+"duplicate-bit/"+std::to_string(N)+"_2").c_str());
            generate(TIME_MAX_EACH, TIME_MAX_TOTAL, PATH + "duplicate-bit/" + std::to_string(N) + "_2", 128, objective,
                     crossover, mutation, selection);
        }
        {
            constexpr size_t N = NF * 9;
            using redundantIndividual = GA::BinaryRepresentation<N>;
            GA::DuplicateBits<Individual, redundantIndividual> objective(realObjective, 0);
            GA::SinglePointCrossover<redundantIndividual> crossover;
            GA::RandomMutation<redundantIndividual> mutation(1. / N);
            GA::ElitismSelection<redundantIndividual> selection(0.05);
            system((MKDIR+PATH+"duplicate-bit/"+std::to_string(N)+"_0").c_str());
            generate(TIME_MAX_EACH, TIME_MAX_TOTAL, PATH + "duplicate-bit/" + std::to_string(N) + "_0", 128, objective,
                     crossover, mutation, selection);
        }
        {
            constexpr size_t N = NF * 9;
            using redundantIndividual = GA::BinaryRepresentation<N>;
            GA::DuplicateBits<Individual, redundantIndividual> objective(realObjective, 1);
            GA::SinglePointCrossover<redundantIndividual> crossover;
            GA::RandomMutation<redundantIndividual> mutation(1. / N);
            GA::ElitismSelection<redundantIndividual> selection(0.05);
            system((MKDIR+PATH+"duplicate-bit/"+std::to_string(N)+"_1").c_str());
            generate(TIME_MAX_EACH, TIME_MAX_TOTAL, PATH + "duplicate-bit/" + std::to_string(N) + "_1", 128, objective,
                     crossover, mutation, selection);
        }
        {
            constexpr size_t N = NF * 9;
            using redundantIndividual = GA::BinaryRepresentation<N>;
            GA::DuplicateBits<Individual, redundantIndividual> objective(realObjective, 2);
            GA::SinglePointCrossover<redundantIndividual> crossover;
            GA::RandomMutation<redundantIndividual> mutation(1. / N);
            GA::ElitismSelection<redundantIndividual> selection(0.05);
            system((MKDIR+PATH+"duplicate-bit/"+std::to_string(N)+"_2").c_str());
            generate(TIME_MAX_EACH, TIME_MAX_TOTAL, PATH + "duplicate-bit/" + std::to_string(N) + "_2", 128, objective,
                     crossover, mutation, selection);
        }
        {
            constexpr size_t N = NF * 11;
            using redundantIndividual = GA::BinaryRepresentation<N>;
            GA::DuplicateBits<Individual, redundantIndividual> objective(realObjective, 0);
            GA::SinglePointCrossover<redundantIndividual> crossover;
            GA::RandomMutation<redundantIndividual> mutation(1. / N);
            GA::ElitismSelection<redundantIndividual> selection(0.05);
            system((MKDIR+PATH+"duplicate-bit/"+std::to_string(N)+"_0").c_str());
            generate(TIME_MAX_EACH, TIME_MAX_TOTAL, PATH + "duplicate-bit/" + std::to_string(N) + "_0", 128, objective,
                     crossover, mutation, selection);
        }
        {
            constexpr size_t N = NF * 11;
            using redundantIndividual = GA::BinaryRepresentation<N>;
            GA::DuplicateBits<Individual, redundantIndividual> objective(realObjective, 1);
            GA::SinglePointCrossover<redundantIndividual> crossover;
            GA::RandomMutation<redundantIndividual> mutation(1. / N);
            GA::ElitismSelection<redundantIndividual> selection(0.05);
            system((MKDIR+PATH+"duplicate-bit/"+std::to_string(N)+"_1").c_str());
            generate(TIME_MAX_EACH, TIME_MAX_TOTAL, PATH + "duplicate-bit/" + std::to_string(N) + "_1", 128, objective,
                     crossover, mutation, selection);
        }
        {
            constexpr size_t N = NF * 11;
            using redundantIndividual = GA::BinaryRepresentation<N>;
            GA::DuplicateBits<Individual, redundantIndividual> objective(realObjective, 2);
            GA::SinglePointCrossover<redundantIndividual> crossover;
            GA::RandomMutation<redundantIndividual> mutation(1. / N);
            GA::ElitismSelection<redundantIndividual> selection(0.05);
            system((MKDIR+PATH+"duplicate-bit/"+std::to_string(N)+"_2").c_str());
            generate(TIME_MAX_EACH, TIME_MAX_TOTAL, PATH + "duplicate-bit/" + std::to_string(N) + "_2", 128, objective,
                     crossover, mutation, selection);
        }
    }

    // Test of information mixing without any redundancy
    for (size_t i: {1, 2, 4, 8, NF/2, NF-1}) {
        using redundantIndividual = GA::BinaryRepresentation<NF>;
        GA::MixInformation<Individual, redundantIndividual> objective(realObjective, i);
        GA::SinglePointCrossover<redundantIndividual> crossover;
        GA::RandomMutation<redundantIndividual> mutation(1. / NF);
        GA::ElitismSelection<redundantIndividual> selection(0.05);
        system((MKDIR+PATH+"mix-information-1.0/"+std::to_string(i)).c_str());
        generate(TIME_MAX_EACH, TIME_MAX_TOTAL, PATH + "mix-information-1.0/"+std::to_string(i), 128, objective, crossover, mutation, selection);
    }

    // Test of information mixing with a redundancy of 1.5
    for (size_t i: {NF/2+1, NF, NF*3/2-1}) {
        using redundantIndividual = GA::BinaryRepresentation<NF*3/2>;
        GA::MixInformation<Individual, redundantIndividual> objective(realObjective, i);
        GA::SinglePointCrossover<redundantIndividual> crossover;
        GA::RandomMutation<redundantIndividual> mutation(1. / NF);
        GA::ElitismSelection<redundantIndividual> selection(0.05);
        system((MKDIR+PATH+"mix-information-1.5/"+std::to_string(i)).c_str());
        generate(TIME_MAX_EACH, TIME_MAX_TOTAL, PATH + "mix-information-1.5/"+std::to_string(i), 128, objective, crossover, mutation, selection);
    }

    // Test of information mixing with a redundancy of 2.0
    for (size_t i: {NF+1, NF*3/2, NF*2-1}) {
        using redundantIndividual = GA::BinaryRepresentation<NF*2>;
        GA::MixInformation<Individual, redundantIndividual> objective(realObjective, i);
        GA::SinglePointCrossover<redundantIndividual> crossover;
        GA::RandomMutation<redundantIndividual> mutation(1. / NF);
        GA::ElitismSelection<redundantIndividual> selection(0.05);
        system((MKDIR+PATH+"mix-information-2.0/"+std::to_string(i)).c_str());
        generate(TIME_MAX_EACH, TIME_MAX_TOTAL, PATH + "mix-information-2.0/"+std::to_string(i), 128, objective, crossover, mutation, selection);
    }

    // Test of information mixing with a redundancy of 3.0
    for (size_t i: {NF*2+1, NF*5/2, NF*3-1}) {
        using redundantIndividual = GA::BinaryRepresentation<NF*3>;
        GA::MixInformation<Individual, redundantIndividual> objective(realObjective, i);
        GA::SinglePointCrossover<redundantIndividual> crossover;
        GA::RandomMutation<redundantIndividual> mutation(1. / NF);
        GA::ElitismSelection<redundantIndividual> selection(0.05);
        system((MKDIR+PATH+"mix-information-3.0/"+std::to_string(i)).c_str());
        generate(TIME_MAX_EACH, TIME_MAX_TOTAL, PATH + "mix-information-3.0/"+std::to_string(i), 128, objective, crossover, mutation, selection);
    }
    return 0;
}

