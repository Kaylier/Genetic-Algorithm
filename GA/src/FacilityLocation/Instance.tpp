#include <algorithm> // swap
#include <cassert>
#include <cstdlib> // size_t
#include <cmath> // sqrt, INFINITY
#include <random>

#include <array>
#include <bitset>
#include <vector>
#include <FacilityLocation/Instance.h>
#include <iostream>

template<size_t NF>
FacilityLocation::Instance<NF>
FacilityLocation::Instance<NF>::randomInstance(size_t numberCustomer, unsigned int seed) {
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
FacilityLocation::Instance<NF>::randomMetricInstance(size_t numberCustomer, unsigned int seed, bool ordered) {
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

    if (ordered) {
        facilityPosition = orderPositions(facilityPosition);
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
FacilityLocation::Instance<NF>
FacilityLocation::Instance<NF>::randomFlawedMetricInstance(size_t numberCustomer, unsigned int seed, bool ordered) {
    std::default_random_engine rnd(seed);
    constexpr int N = 3; // Number of subdivision on each coordinate
    std::uniform_real_distribution<double> distrib(0., 1./N);
    std::array<double, N*N> subdivision_prob = {
            0.16,0.08,0.16,
            0.08,0.04,0.08,
            0.16,0.08,0.16};
/*
    std::generate(subdivision_prob.begin(), subdivision_prob.end(), [&distrib, &rnd]() {return distrib(rnd);});
    for (double x: subdivision_prob) {
        std::cout << x << std::endl;
    }
*/
    std::discrete_distribution<int> subdivision_distrib(subdivision_prob.begin(), subdivision_prob.end());
    /*
     * We associate a 2D position to each customer and facility, and then convert to distances
     */
    Instance out(numberCustomer);

    using Coordinate = std::pair<double, double>;
    std::vector<Coordinate> customerPosition;
    for (size_t n = 0; n < numberCustomer; ++n) {
        int subdiv = subdivision_distrib(rnd);
        customerPosition.push_back(Coordinate(subdiv%3+distrib(rnd), subdiv/3+distrib(rnd)));
    }
    std::vector<std::pair<double, double>> facilityPosition;
    for (size_t n = 0; n < numberFacility; ++n) {
        int subdiv = subdivision_distrib(rnd);
        facilityPosition.push_back(Coordinate(subdiv%3+distrib(rnd), subdiv/3+distrib(rnd)));
    }

    if (ordered) {
        facilityPosition = orderPositions(facilityPosition);
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
        out.openingCost[iF] = distrib(rnd)*10. + 10.;
    }
    return out;
}

template<size_t NF>
FacilityLocation::Instance<NF> FacilityLocation::Instance<NF>::load(std::string filename) {
    // TODO: safe loading, no check are done
    std::ifstream file(filename);
    std::string str;
    size_t numberFacility, numberCustomer;
    double input;

    file >> str;
    assert(str == "Size:");
    file >> numberFacility;
    assert(numberFacility == NF);
    file >> numberCustomer;

    FacilityLocation::Instance<NF> instance(numberCustomer);
    for (size_t iF = 0; iF < numberFacility; ++iF) {
        instance.distances[iF] = new double[numberCustomer];
    }

    file >> str;
    assert(str == "Distances:");
    for (size_t i = 0; i < numberFacility; ++i) {
        for (size_t j = 0; j < numberCustomer; ++j) {
            file >> input;
            instance.distances[i][j] = input;
        }
    }


    file >> str;
    assert(str == "Opening");
    file >> str;
    assert(str == "costs:");
    for (size_t i = 0; i < numberFacility; ++i) {
        file >> input;
        instance.openingCost[i] = input;
    }

    file.close();
    return instance;
}

template<size_t NF>
std::vector<std::pair<double, double>> FacilityLocation::Instance<NF>::orderPositions(std::vector<std::pair<double, double>> input) {
    std::vector<std::pair<double, double>> result;
    /* Get a random element as initialization
     * Get the nearest element and add it
     * Repeat until elements are left
     */

    result.push_back(input.back());
    input.pop_back();

    while (input.size() > 0) {
        double minDist = 10.;
        int argmin = 0;
        for (size_t i = 0; i < input.size(); i++) {
            double dx = input[i].first - result.back().first;
            double dy = input[i].second - result.back().second;
            if (dx * dx + dy * dy < minDist) {
                minDist = dx*dx + dy*dy;
                argmin = i;
            }
        }
        result.push_back(input[argmin]);
        input.erase(input.begin()+argmin);
    }

    return result;
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
FacilityLocation::Instance<NF> &
FacilityLocation::Instance<NF>::operator=(const FacilityLocation::Instance<NF> &instance) {
    Instance &copy = FacilityLocation::Instance<NF>(instance);
    swap(*this, copy);
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
    std::ofstream file(filename);
    file << "Size: " << numberFacility << "\t" << numberCustomer << std::endl;
    file << "Distances: " << std::endl;
    for (size_t i = 0; i < numberFacility; ++i) {
        for (size_t j = 0; j < numberCustomer; ++j) {
            file << distance(i, j) << "\t";
        }
        file << std::endl;
    }
    file << "Opening costs: " << std::endl;
    for (size_t i = 0; i < numberFacility; ++i) {
        file << cost(i) << "\t";
    }
    file << std::endl;
    file.close();
}

