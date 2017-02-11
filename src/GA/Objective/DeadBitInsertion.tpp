#include <cassert>
#include <GA/Representation/BinaryRepresentation.h>
#include <GA/Objective/DeadBitInsertion.h>

template<size_t N, size_t M>
GA::DeadBitInsertion<GA::BinaryRepresentation<N>, GA::BinaryRepresentation<M>>::DeadBitInsertion(
        GA::Objective<GA::BinaryRepresentation<N>> &initialObjective, size_t position) :
        initialObjective(initialObjective),
        position(position) {
    assert(position < N);
}

template<size_t N, size_t M>
double GA::DeadBitInsertion<GA::BinaryRepresentation<N>, GA::BinaryRepresentation<M>>::operator()(
        const GA::BinaryRepresentation<M> &individual) {
    GA::BinaryRepresentation<N> initialIndividual;
    size_t i;
    for (i = 0; i < position; i++) {
        initialIndividual.set(i, individual[i]);
    }
    for (; i < N; i++) {
        initialIndividual.set(i, individual[i + length]);
    }
    return initialObjective(initialIndividual);
}
