#ifndef FACILITYLOCATION_OBJECTIVE_H
#define FACILITYLOCATION_OBJECTIVE_H

#include <bitset>
#include "GA/Objective.h"
#include "FacilityLocation/Instance.h"
#include "GA/Individual/BinaryRepresentation.h"

namespace FacilityLocation {

    template<class Individual>
    class Objective;

    template<size_t N>
    class Objective<GA::BinaryRepresentation<N>> final : public GA::Objective<GA::BinaryRepresentation<N>> {

    public:
        using Individual = GA::BinaryRepresentation<N>;

    public:
        Objective(const Instance<N> &instance);
        ~Objective() = default;

        const Instance<N> &getInstance() const;

        double operator()(const Individual &individual) override;

    private:
        const Instance<N> &instance;

    };

    template class Objective<GA::BinaryRepresentation<4>>;

}

#endif //FACILITYLOCATION_OBJECTIVE_H
