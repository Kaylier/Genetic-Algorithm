#include <cmath> // INFINITY

template<size_t N>
FacilityLocation::Objective<GA::BinaryRepresentation<N>>::Objective(const FacilityLocation::Instance<N> &instance) :
        instance(instance) {
}

template<size_t N>
const FacilityLocation::Instance<N> &FacilityLocation::Objective<GA::BinaryRepresentation<N>>::getInstance() const {
    return instance;
}

template<size_t N>
double FacilityLocation::Objective<GA::BinaryRepresentation<N>>::operator()(
        const FacilityLocation::Objective<GA::BinaryRepresentation<N>>::Individual &individual) {
    double result = 0.;
    for (size_t nF = 0; nF < instance.getNumberFacility(); ++nF) {
        if (individual[nF]) {
            result += instance.cost(nF);
        }
    }
    double min;
    for (size_t nC = 0; nC < instance.getNumberCustomer(); ++nC) {
        min = INFINITY;
        for (size_t nF = 0; nF < instance.getNumberFacility(); ++nF) {
            if (individual[nF] && instance.distance(nF, nC) < min) {
                min = instance.distance(nF, nC);
            }
        }
        result += min;
    }
    return result;
}
