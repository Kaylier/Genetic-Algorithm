#ifndef GENETICALGORITHM_CROSSOVER_H
#define GENETICALGORITHM_CROSSOVER_H

#include <type_traits> // is_base_of

#include "GA/Representation.h"

namespace GA {

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

        virtual Individual operator()(const Individual &individual1, const Individual &individual2) = 0;

    };

    template<class Individual>
    inline Crossover<Individual>::~Crossover() {}

}

#endif //GENETICALGORITHM_CROSSOVER_H
