#include <cassert>
#include <random>

template <size_t N>
GA::BinaryRepresentation<N>::BinaryRepresentation() {
    std::random_device rndDevice;
    this->rnd.seed(rndDevice());
}

template<size_t N>
GA::BinaryRepresentation<N>::BinaryRepresentation(unsigned long val) : Representation(), std::bitset<N>(val) {
    std::random_device rndDevice;
    this->rnd.seed(rndDevice());
}

template<size_t N>
void GA::BinaryRepresentation<N>::randomize() {
    std::bernoulli_distribution distrib(0.5);
    size_t i = 0;
    while (i < N) {
        if (distrib(rnd)) {
            this->flip(i);
        }
        ++i;
    }
}
