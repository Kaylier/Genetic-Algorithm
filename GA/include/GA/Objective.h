#ifndef GENETICALGORITHM_OBJECTIVE_H
#define GENETICALGORITHM_OBJECTIVE_H

#include <type_traits> // is_base_of

#include "GA/Representation.h"

namespace GA {

    /**
     * Interface of a objective functor that can be bound to a GA::Engine.
     * The functor must implement an operator() which returns the value associated
     * to the given individual.
     * @tparam Individual Type of individuals, must be a subclass of Representation
     */
    template<class Individual>
    class Objective {
        static_assert(std::is_base_of<Representation, Individual>::value,
                      "Individual not derived from Representation");

    public:
        Objective() = default;
        Objective(const Objective&) = default;
        Objective(Objective&&) = default;
        virtual ~Objective() = 0;

        Objective &operator=(const Objective&) = default;
        Objective &operator=(Objective&&) = default;

        /**
         * Compute the objective value of an individual
         * @param individual Individual to evaluate
         * @return The value of the individual
         */
        virtual double operator()(const Individual &individual) = 0;

    };

    template<class Individual>
    inline Objective<Individual>::~Objective() {}

}

#endif //GENETICALGORITHM_OBJECTIVE_H
