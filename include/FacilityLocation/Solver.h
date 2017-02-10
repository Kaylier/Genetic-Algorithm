#ifndef GENETICALGORITHM_SOLVER_H
#define GENETICALGORITHM_SOLVER_H

#include "Objective.h"

namespace FacilityLocation {

    template<size_t NF>
    class Solver {

    public:
        static double bruteForce(Instance<NF> &instance, Objective<GA::BinaryRepresentation<NF>> &objective);
        static double greedy(Instance<NF> &instance);

    };

}

#include "FacilityLocation/Solver.tpp"

#endif //GENETICALGORITHM_SOLVER_H
