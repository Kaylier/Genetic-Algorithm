#ifndef GENETICALGORITHM_BINARYREPRESENTATION_H
#define GENETICALGORITHM_BINARYREPRESENTATION_H

#include <bitset>
#include <random>

#include "GA/Representation.h"

namespace GA {

    /**
     * TODO: documentation
     * @tparam N Number of bits
     */
    template<size_t N>
    class BinaryRepresentation : public Representation, public std::bitset<N> {

    public:
        BinaryRepresentation();
        BinaryRepresentation(const BinaryRepresentation&) = default;
        BinaryRepresentation(BinaryRepresentation&&) = default;
        BinaryRepresentation(unsigned long val);
        ~BinaryRepresentation() = default;

        BinaryRepresentation &operator=(BinaryRepresentation&) = default;
        BinaryRepresentation &operator=(BinaryRepresentation&&) = default;

        void randomize() override;

    private:
        std::default_random_engine rnd;

    };

}

#include "GA/Representation/BinaryRepresentation.tpp"

#endif //GENETICALGORITHM_BINARYINDIVIDUAL_H
