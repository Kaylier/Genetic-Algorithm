#ifndef GENETICALGORITHM_DEADBITINSERTION_H
#define GENETICALGORITHM_DEADBITINSERTION_H

#include <cstdlib>
#include <GA/Representation/BinaryRepresentation.h>
#include <FacilityLocation/Objective.h>

namespace GA {

    template<class initialIndividual, class modifiedIndividual>
    class DeadBitInsertion;

    template<size_t N, size_t M>
    class DeadBitInsertion<GA::BinaryRepresentation<N>, GA::BinaryRepresentation<M>> : public GA::Objective<GA::BinaryRepresentation<M>> {
        static_assert(M >= N, "Incompatible size of binary representation");

    public:
        DeadBitInsertion() = delete;

        DeadBitInsertion(const DeadBitInsertion&) = default;
        DeadBitInsertion(DeadBitInsertion&&) = default;
        DeadBitInsertion(GA::Objective<GA::BinaryRepresentation<N>> &initialObjective, size_t position = 0);

        ~DeadBitInsertion() = default;

        DeadBitInsertion& operator=(const DeadBitInsertion&) = default;
        DeadBitInsertion& operator=(DeadBitInsertion&&) = default;

        double operator()(const GA::BinaryRepresentation<M> &individual);

    private:
        Objective<GA::BinaryRepresentation<N>> &initialObjective;
        const size_t position;
        static constexpr size_t length = M - N;

    };

}

#include "GA/Objective/DeadBitInsertion.tpp"

#endif //GENETICALGORITHM_DEADBITINSERTION_H
