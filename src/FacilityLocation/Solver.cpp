#include <cmath> // INFINITY
#include <iostream> // cerr, endl

#include "FacilityLocation/Solver.h"

template<size_t NF>
double FacilityLocation::Solver<NF>::bruteForce(FacilityLocation::Instance<NF> &instance,
                                            FacilityLocation::Objective<GA::BinaryRepresentation<NF>> &objective) {
    if (instance.getNumberFacility() > sizeof(int)) {
        std::cerr << "[ERROR] Bruteforce asked for a too big instance (" << instance.getNumberFacility() << ")"
                  << ". The maximal size authorized is " << sizeof(int) << std::endl;
    }
    double bestScore = INFINITY;
    for (unsigned int n = 1; n <= (int)pow(2, instance.numberFacility); ++n) {
        GA::BinaryRepresentation<NF> individual(n);
        double score = objective(individual);
        if (score < bestScore) {
            bestScore = score;
        }
    }
    return bestScore;
}

template<size_t NF>
double FacilityLocation::Solver<NF>::greedy(FacilityLocation::Instance<NF> &instance) {
    // TODO: greedy algorithm
    return 0;
}
