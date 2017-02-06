#ifndef GENETICALGORITHM_SINGLEPOINTCROSSOVER_H
#define GENETICALGORITHM_SINGLEPOINTCROSSOVER_H

#include "GA/Crossover.h"
#include "GA/Individual/BinaryRepresentation.h"

namespace GA {

    /**
     * TODO: documentation
     * @tparam Individual Type of individuals, must be a subclass of Representation
     */
    template<class Individual>
    class SinglePointCrossover;

    template<size_t N>
    class SinglePointCrossover<BinaryRepresentation<N>> : public Crossover<BinaryRepresentation<N>> {

    public:
        SinglePointCrossover() = default;
        SinglePointCrossover(const SinglePointCrossover&) = default;
        SinglePointCrossover(SinglePointCrossover&&) = default;
        virtual ~SinglePointCrossover() = default;

        SinglePointCrossover &operator=(const SinglePointCrossover&) = default;
        SinglePointCrossover &operator=(SinglePointCrossover&&) = default;

        BinaryRepresentation<N>
        operator()(const BinaryRepresentation<N> &individual1, const BinaryRepresentation<N> &individual2) override;

    };

    template class SinglePointCrossover<BinaryRepresentation<4>>;

}

#endif //GENETICALGORITHM_SINGLEPOINTCROSSOVER_H
