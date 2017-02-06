#ifndef GENETICALGORITHM_BINARYREPRESENTATION_H
#define GENETICALGORITHM_BINARYREPRESENTATION_H

#include <bitset>
#include "GA/Representation.h"

namespace GA {

    /**
     * TODO: documentation
     * @tparam N Number of bits
     */
    template<size_t N>
    class BinaryRepresentation : public Representation, public std::bitset<N> {

    public:
        BinaryRepresentation() = default;
        BinaryRepresentation(const BinaryRepresentation&) = default;
        BinaryRepresentation(BinaryRepresentation&&) = default;
        BinaryRepresentation(unsigned long val);
        ~BinaryRepresentation() = default;

        BinaryRepresentation &operator=(BinaryRepresentation&) = default;
        BinaryRepresentation &operator=(BinaryRepresentation&&) = default;

        void randomize() override;

    };

}

#include "GA/Representation/BinaryRepresentation.tpp"

#endif //GENETICALGORITHM_BINARYINDIVIDUAL_H
