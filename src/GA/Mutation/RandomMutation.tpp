#include <cassert>

#include "GA/Mutation/RandomMutation.h"

template<size_t N>
GA::RandomMutation<GA::BinaryRepresentation<N>>::RandomMutation(double probability) {
    this->setProbability(probability);
    std::random_device rndDevice;
    this->rnd.seed(rndDevice());
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
    std::uniform_real_distribution<double> probaRand(0., 1.);
    for (size_t n = 0; n < individual.size(); ++n) {
        if (probaRand(rnd) < probability) {
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
