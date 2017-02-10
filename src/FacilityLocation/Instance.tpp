#include <algorithm> // swap
#include <cassert>
#include <cstdlib> // size_t
#include <cmath> // sqrt, INFINITY
#include <random>

#include <bitset>
#include <vector>

#include "FacilityLocation/Instance.h"

template<size_t NF>
FacilityLocation::Instance<NF> FacilityLocation::Instance<NF>::randomInstance(size_t numberCustomer, unsigned int seed) {
    std::default_random_engine rnd(seed);
    std::uniform_real_distribution<double> distrib(0., 1.);

    Instance out(numberCustomer);
    // iF for index of the facility
    for (size_t iF = 0; iF < numberFacility; ++iF) {
        out.distances[iF] = new double[numberCustomer];
        // iC for index of customer
        for (size_t iC = 0; iC < numberCustomer; ++iC) {
            out.distances[iF][iC] = distrib(rnd);
        }
    }
    for (size_t iF = 0; iF < numberFacility; ++iF) {
        out.openingCost[iF] = distrib(rnd);
    }
    return out;
}

template<size_t NF>
FacilityLocation::Instance<NF>
FacilityLocation::Instance<NF>::randomMetricInstance(size_t numberCustomer, unsigned int seed) {
    std::default_random_engine rnd(seed);
    std::uniform_real_distribution<double> distrib(0., 1.);
    /*
     * We associate a 2D position to each customer and facility, and then convert to distances
     */
    Instance out(numberCustomer);

    using Coordinate = std::pair<double, double>;
    std::vector<Coordinate> customerPosition;
    for (size_t n = 0; n < numberCustomer; ++n) {
        customerPosition.push_back(Coordinate(distrib(rnd), distrib(rnd)));
    }
    std::vector<std::pair<double, double>> facilityPosition;
    for (size_t n = 0; n < numberFacility; ++n) {
        facilityPosition.push_back(Coordinate(distrib(rnd), distrib(rnd)));
    }

    double x, y;
    // iF for index of facility
    for (size_t iF = 0; iF < numberFacility; ++iF) {
        out.distances[iF] = new double[numberCustomer];
        // iC for index of customer
        for (size_t iC = 0; iC < numberCustomer; ++iC) {
            x = facilityPosition[iC].first - customerPosition[iF].first;
            y = facilityPosition[iC].second - customerPosition[iF].second;
            out.distances[iF][iC] = sqrt(x * x + y * y);
        }
    }

    for (size_t iF = 0; iF < numberFacility; ++iF) {
        out.openingCost[iF] = distrib(rnd);
    }
    return out;
}

template<size_t NF>
FacilityLocation::Instance<NF> FacilityLocation::Instance<NF>::load(std::string filename) {
    // TODO: load from file
    return randomMetricInstance(16);
}

template<size_t NF>
FacilityLocation::Instance<NF>::Instance(size_t numberCustomer) : numberCustomer(numberCustomer) {}

template<size_t NF>
FacilityLocation::Instance<NF>::Instance(const FacilityLocation::Instance<NF> &instance) :
        numberCustomer(instance.numberCustomer) {
    // iF for index of the facility
    for (size_t iF = 0; iF < this->numberFacility; ++iF) {
        this->distances[iF] = new double[this->numberCustomer];
        // iC for index of customer
        for (size_t iC = 0; iC < this->numberCustomer; ++iC) {
            this->distances[iF][iC] = instance.distances[iF][iC];
        }
    }
    for (size_t iF = 0; iF < this->numberFacility; ++iF) {
        this->openingCost[iF] = instance.openingCost[iF];
    }
}

template<size_t NF>
FacilityLocation::Instance<NF>::Instance(FacilityLocation::Instance<NF> &&instance) : numberCustomer(
        instance.numberCustomer) {
    swap(*this, instance);
}

template<size_t NF>
FacilityLocation::Instance<NF>::~Instance() {
    for (size_t i = 0; i < numberFacility; ++i) {
        delete[] distances[i];
    }
}

template<size_t NF>
FacilityLocation::Instance<NF> &FacilityLocation::Instance<NF>::operator=(FacilityLocation::Instance<NF> instance) {
    swap(*this, instance);
    return *this;
}

template<size_t NF>
FacilityLocation::Instance<NF> &FacilityLocation::Instance<NF>::operator=(FacilityLocation::Instance<NF> &&instance) {
    swap(*this, instance);
    return *this;
}

template<size_t NF>
size_t FacilityLocation::Instance<NF>::getNumberFacility() const {
    return numberFacility;
}

template<size_t NF>
size_t FacilityLocation::Instance<NF>::getNumberCustomer() const {
    return numberCustomer;
}

template<size_t NF>
double FacilityLocation::Instance<NF>::distance(size_t facility, size_t customer) const {
    assert(facility < numberFacility);
    assert(customer < numberCustomer);
    return distances[facility][customer];
}

template<size_t NF>
double FacilityLocation::Instance<NF>::cost(size_t facility) const {
    assert(facility < numberFacility);
    return openingCost[facility];
}

template<size_t NF>
void FacilityLocation::swap(FacilityLocation::Instance<NF> &first, FacilityLocation::Instance<NF> &second) {
    assert(first.numberCustomer == second.numberCustomer);
    using std::swap;
//    swap(first.numberFacility, second.numberFacility);
//    swap(first.numberCustomer, second.numberCustomer);
    swap(first.distances, second.distances);
    swap(first.openingCost, second.openingCost);
}

template<size_t NF>
void FacilityLocation::Instance<NF>::save(std::string filename) const {
    // TODO: save to file
}

