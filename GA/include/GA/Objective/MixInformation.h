#ifndef GENETICALGORITHM_MIXINFORMATION_H
#define GENETICALGORITHM_MIXINFORMATION_H

#include <cstdlib>
#include <GA/Representation/BinaryRepresentation.h>
#include <FacilityLocation/Objective.h>

namespace GA {

    template<class initialIndividual, class modifiedIndividual>
    class MixInformation;

    template<size_t N, size_t M>
    class MixInformation<GA::BinaryRepresentation<N>, GA::BinaryRepresentation<M>> : public GA::Objective<GA::BinaryRepresentation<M>> {
        static_assert(M >= N, "Incompatible size of binary representation");

    public:
        MixInformation() = delete;

        MixInformation(const MixInformation&) = default;
        MixInformation(MixInformation&&) = default;
        MixInformation(GA::Objective<GA::BinaryRepresentation<N>> &initialObjective, size_t range);

        ~MixInformation() = default;

        MixInformation& operator=(const MixInformation&) = default;
        MixInformation& operator=(MixInformation&&) = default;

        double operator()(const GA::BinaryRepresentation<M> &individual);

    private:
        Objective<GA::BinaryRepresentation<N>> &initialObjective;
        const size_t range;

    };

}

#include "GA/Objective/MixInformation.tpp"

#endif //GENETICALGORITHM_MIXINFORMATION_H
