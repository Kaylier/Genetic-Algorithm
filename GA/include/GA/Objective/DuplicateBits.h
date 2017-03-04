#ifndef GENETICALGORITHM_DUPLICATEBITS_H
#define GENETICALGORITHM_DUPLICATEBITS_H

#include <cstdlib>
#include <GA/Representation/BinaryRepresentation.h>
#include <FacilityLocation/Objective.h>

namespace GA {

    template<class initialIndividual, class modifiedIndividual>
    class DuplicateBits;

    template<size_t N, size_t M>
    class DuplicateBits<GA::BinaryRepresentation<N>, GA::BinaryRepresentation<M>> : public GA::Objective<GA::BinaryRepresentation<M>> {
        static_assert(M % N == 0, "Incompatible size of binary representation");

    public:
        DuplicateBits() = delete;

        DuplicateBits(const DuplicateBits&) = default;
        DuplicateBits(DuplicateBits&&) = default;
        DuplicateBits(GA::Objective<GA::BinaryRepresentation<N>> &initialObjective, size_t offset);

        ~DuplicateBits() = default;

        DuplicateBits& operator=(const DuplicateBits&) = default;
        DuplicateBits& operator=(DuplicateBits&&) = default;

        double operator()(const GA::BinaryRepresentation<M> &individual);

    private:
        Objective<GA::BinaryRepresentation<N>> &initialObjective;
        const size_t offset;
        static constexpr unsigned int redundancy = M / N;

    };

}

#include "GA/Objective/DuplicateBits.tpp"

#endif //GENETICALGORITHM_DUPLICATEBITS_H
