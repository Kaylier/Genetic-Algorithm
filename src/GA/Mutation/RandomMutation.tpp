#include <cassert>
#include <cstdlib> // rand

#include "GA/Mutation/RandomMutation.h"

template<size_t N>
GA::RandomMutation<GA::BinaryRepresentation<N>>::RandomMutation(double probability) {
    this->setProbability(probability);
}

template<size_t N>
double GA::RandomMutation<GA::BinaryRepresentation<N>>::getProbability() const {
    return probability;
}

template<size_t N>
void GA::RandomMutation<GA::BinaryRepresentation<N>>::setProbability(double probability) {
    assert(0. <= probability && probability <= 1.);
    this->probability = probability;
}

template<size_t N>
typename GA::RandomMutation<GA::BinaryRepresentation<N>>::Individual
GA::RandomMutation<GA::BinaryRepresentation<N>>::operator()(
        GA::RandomMutation<GA::BinaryRepresentation<N>>::Individual &individual) {
    for (int n = 0; n < individual.size(); ++n) {
        if (rand() < probability * RAND_MAX) {
            individual.flip(n);
        }
    }
    return individual;
}

template<size_t N>
typename GA::RandomMutation<GA::BinaryRepresentation<N>>::Individual
GA::RandomMutation<GA::BinaryRepresentation<N>>::operator()(
        const GA::RandomMutation<GA::BinaryRepresentation<N>>::Individual &individual) {
    GA::RandomMutation<GA::BinaryRepresentation<N>>::Individual newIndividual(individual);
    this->operator()(newIndividual);
    return newIndividual;
}
