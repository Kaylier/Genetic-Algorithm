#include <cassert>
#include <GA/Representation/BinaryRepresentation.h>
#include <GA/Objective/MixInformation.h>

template<size_t N, size_t M>
GA::MixInformation<GA::BinaryRepresentation<N>, GA::BinaryRepresentation<M>>::MixInformation(
        GA::Objective<GA::BinaryRepresentation<N>> &initialObjective, size_t range) :
        initialObjective(initialObjective),
        range(range) {
    assert(range > M-N); // Otherwise there is unused bits
    assert(range < M);
}

template<size_t N, size_t M>
double GA::MixInformation<GA::BinaryRepresentation<N>, GA::BinaryRepresentation<M>>::operator()(
        const GA::BinaryRepresentation<M> &individual) {
    GA::BinaryRepresentation<N> initialIndividual;
    size_t i, j;
    for (i = 0; i < N; i++) {
        initialIndividual.set(i, false);
        for (j = 0; j < range; j++) {
            if (individual[(i + j) % M]) {
                initialIndividual.flip(i);
            }
        }
    }
    return initialObjective(initialIndividual);
}
