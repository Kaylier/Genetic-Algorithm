#ifndef GENETICALGORITHM_RANDOMMUTATION_H
#define GENETICALGORITHM_RANDOMMUTATION_H

#include "GA/Mutation.h"
#include "GA/Representation/BinaryRepresentation.h"

namespace GA {

    /**
     * TODO: documentation
     * @tparam Individual Type of individuals, must be a subclass of Representation
     */
    template<class Individual>
    class RandomMutation;

    template<size_t N>
    class RandomMutation<BinaryRepresentation<N>> : public Mutation<BinaryRepresentation<N>> {

    public:
        using Individual = BinaryRepresentation<N>;

    public:
        RandomMutation() = delete;

        RandomMutation(const RandomMutation&) = default;
        RandomMutation(RandomMutation&&) = default;
        RandomMutation(double probability);
        virtual ~RandomMutation() = default;

        RandomMutation &operator=(const RandomMutation&) = default;
        RandomMutation &operator=(RandomMutation&&) = default;

        double getProbability() const;
        void setProbability(double probability);

        Individual operator()(Individual &individual) override;
        Individual operator()(const Individual &individual) override;

    protected:
        double probability;

    };

}

#include "GA/Mutation/RandomMutation.tpp"

#endif //GENETICALGORITHM_RANDOMMUTATION_H
