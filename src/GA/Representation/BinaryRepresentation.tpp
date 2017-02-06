#include <cstdlib> // rand

#include "GA/Representation/BinaryRepresentation.h"

template<size_t N>
GA::BinaryRepresentation<N>::BinaryRepresentation(unsigned long val) : Representation(), std::bitset<N>(val) {}

template<size_t N>
void GA::BinaryRepresentation<N>::randomize() {
    size_t i = 0;
    int val;
    while (i < N) {
        val = rand();
        for (int b = 0; b < sizeof(int); ++b) {
            this->flip(i);
            ++i;
        }
    }
}
