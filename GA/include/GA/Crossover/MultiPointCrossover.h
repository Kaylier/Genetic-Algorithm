#ifndef GENETICALGORITHM_MULTIPOINTCROSSOVER_H
#define GENETICALGORITHM_MULTIPOINTCROSSOVER_H

#include <random>

#include "GA/Crossover.h"
#include "GA/Representation/BinaryRepresentation.h"

namespace GA {

    /**
     * TODO: documentation
     * @tparam Individual Type of individuals, must be a subclass of Representation
     */
    template<class Individual>
    class MultiPointCrossover;

    template<size_t N>
    class MultiPointCrossover<BinaryRepresentation<N>> : public Crossover<BinaryRepresentation<N>> {

    public:
        const unsigned int numberPoint;

        MultiPointCrossover() = delete;

        MultiPointCrossover(const MultiPointCrossover&) = default;
        MultiPointCrossover(MultiPointCrossover&&) = default;
        MultiPointCrossover(const unsigned int numberPoint = 1);
        virtual ~MultiPointCrossover() = default;

        MultiPointCrossover &operator=(const MultiPointCrossover&) = default;
        MultiPointCrossover &operator=(MultiPointCrossover&&) = default;

        BinaryRepresentation<N>
        operator()(const BinaryRepresentation<N> &individual1, const BinaryRepresentation<N> &individual2) override;

    private:
        std::default_random_engine rnd;

    };

}

#include "GA/Crossover/MultiPointCrossover.tpp"

#endif //GENETICALGORITHM_SINGLEPOINTCROSSOVER_H
