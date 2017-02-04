#include <cstdlib> // rand

#include "GA/Crossover/SinglePointCrossover.h"

template<size_t N>
GA::BinaryRepresentation<N>
GA::SinglePointCrossover<GA::BinaryRepresentation<N>>::operator()(const GA::BinaryRepresentation<N> &individual1,
                                                               const GA::BinaryRepresentation<N> &individual2) {
    BinaryRepresentation<N> child;
    size_t n;
    size_t point = rand() % N;

    for (n = 0; n < point; ++n) {
        child.set(n, individual1[n]);
    }
    for (; n < N; ++n) {
        child.set(n, individual2[n]);
    }
    return child;
}
