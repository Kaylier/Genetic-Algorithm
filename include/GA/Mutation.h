#ifndef GENETICALGORITHM_MUTATION_H
#define GENETICALGORITHM_MUTATION_H

#include <type_traits> // is_base_of

#include "GA/Representation.h"

namespace GA {

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

        virtual Individual operator()(Individual &individual) = 0;
        virtual Individual operator()(const Individual &individual) = 0;

    };

    template<class Individual>
    inline Mutation<Individual>::~Mutation() {}

}

#endif //GENETICALGORITHM_MUTATION_H
