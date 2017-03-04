#include <cassert>
#include <GA/Representation/BinaryRepresentation.h>
#include <GA/Objective/DuplicateBits.h>

template<size_t N, size_t M>
GA::DuplicateBits<GA::BinaryRepresentation<N>, GA::BinaryRepresentation<M>>::DuplicateBits(
        GA::Objective<GA::BinaryRepresentation<N>> &initialObjective, size_t offset) :
        initialObjective(initialObjective),
        offset(offset) {
}

template<size_t N, size_t M>
double GA::DuplicateBits<GA::BinaryRepresentation<N>, GA::BinaryRepresentation<M>>::operator()(
        const GA::BinaryRepresentation<M> &individual) {
    GA::BinaryRepresentation<N> initialIndividual;
    size_t i, j;
    int value;
    for (i = 0; i < N; i++) {
        value = 0;
        for (j = 0; j < redundancy; j++) {
            value += (individual[(i*redundancy + j*redundancy*offset + j) % M])? 1 : -1;
        }
        initialIndividual.set(i, value >= 0);
    }
    return initialObjective(initialIndividual);
}
