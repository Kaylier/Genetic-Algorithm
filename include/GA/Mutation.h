#ifndef GENETICALGORITHM_MUTATION_H
#define GENETICALGORITHM_MUTATION_H

#include <type_traits> // is_base_of

#include "GA/Representation.h"

namespace GA {

    /**
     * Interface of a mutation functor that can be bound to a GA::Engine.
     * The functor must implement an operator() which returns the given individual after
     * its modification.
     * @tparam Individual Type of individuals, must be a subclass of Representation
     */
    template<class Individual>
    class Mutation {
        static_assert(std::is_base_of<Representation, Individual>::value,
                      "Individual not derived from Representation");

    public:
        Mutation() = default;
        Mutation(const Mutation&) = default;
        Mutation(Mutation&&) = default;
        virtual ~Mutation() = 0;

        Mutation &operator=(const Mutation&) = default;
        Mutation &operator=(Mutation&&) = default;

        /**
         * Modify the individual.
         * @param individual Individual to modify
         * @return The modified individual
         */
        virtual Individual operator()(Individual &individual) = 0;
        /**
         * Generate a modified version of an individual
         * @param individual Individual to modify
         * @return A new individual
         */
        virtual Individual operator()(const Individual &individual) = 0;

    };

    template<class Individual>
    inline Mutation<Individual>::~Mutation() {}

}

#endif //GENETICALGORITHM_MUTATION_H
