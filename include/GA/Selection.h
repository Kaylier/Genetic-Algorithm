#ifndef GENETICALGORITHM_SELECTION_H
#define GENETICALGORITHM_SELECTION_H

#include <type_traits> // is_base_of

#include <map>

#include "GA/Representation.h"

namespace GA {

    /**
     * Interface of a selection functor that can be bound to a GA::Engine.
     * The functor must implement an operator() which compute a selection on a population.
     * @tparam Individual Type of individuals, must be a subclass of Representation
     */
    template<class Individual>
    class Selection {
        static_assert(std::is_base_of<Representation, Individual>::value,
                      "Individual not derived from Representation");

    public:
        using Population = std::multimap<double, Individual>;

    public:
        Selection() = default;
        Selection(const Selection&) = default;
        Selection(Selection&&) = default;
        virtual ~Selection() = 0;

        Selection &operator=(const Selection&) = default;
        Selection &operator=(Selection&&) = default;

        /**
         * Perform a selection on a population by modifying the argument.
         * Some individual may be deleted, but none are added.
         * The final population is guaranted to have at least one individual.
         * @param population The initial population
         * @return The population with at least one individual
         */
        virtual Population operator()(Population &population) = 0;
        /**
         * Perform a selection on a population. The given population is not modified
         * Every individuals in the final population are in the initial population.
         * The final population is guaranted to have at least one individual.
         * @param population The initial population
         * @return A population with at least one individual
         */
        virtual Population operator()(const Population &population) = 0;

    };

    template<class Individual>
    inline Selection<Individual>::~Selection() {}

}

#endif //GENETICALGORITHM_SELECTION_H
