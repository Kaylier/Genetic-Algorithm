#ifndef GENETICALGORITHM_CROSSOVER_H
#define GENETICALGORITHM_CROSSOVER_H

#include <type_traits> // is_base_of

#include "GA/Representation.h"

namespace GA {

    /**
     * Interface of a crossover functor that can be bound to a GA::Engine.
     * The functor must implement an operator() which mix two individuals to create
     * a new one.
     * @tparam Individual Type of individuals, must be a subclass of Representation
     */
    template<class Individual>
    class Crossover {
        static_assert(std::is_base_of<Representation, Individual>::value,
                      "Individual not derived from Representation");

    public:
        Crossover() = default;
        Crossover(const Crossover&) = default;
        Crossover(Crossover&&) = default;
        virtual ~Crossover() = 0;

        Crossover &operator=(const Crossover&) = default;
        Crossover &operator=(Crossover&&) = default;

        /**
         * Generate a new individual based on two given ones.
         * @param individual1 First individual
         * @param individual2 Second individual
         * @return A new individual
         */
        virtual Individual operator()(const Individual &individual1, const Individual &individual2) = 0;

    };

    template<class Individual>
    inline Crossover<Individual>::~Crossover() {}

}

#endif //GENETICALGORITHM_CROSSOVER_H
