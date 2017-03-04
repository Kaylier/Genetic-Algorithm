#include <cmath> // INFINITY
#include <iostream> // cerr, endl
#include <vector>
#include <FacilityLocation/Instance.h>

template<size_t NF>
double FacilityLocation::Solver<NF>::bruteForce(FacilityLocation::Instance<NF> &instance,
                                                FacilityLocation::Objective<GA::BinaryRepresentation<NF>> &objective) {
    if (instance.getNumberFacility() > sizeof(int) * 8) {
        std::cerr << "[ERROR] Bruteforce asked for a too big instance (" << instance.getNumberFacility() << ")"
                  << ". The maximal size authorized is " << sizeof(int) * 8 << std::endl;
    }
    double bestScore = INFINITY;
    for (unsigned int n = 1; n <= (int) pow(2, instance.numberFacility); ++n) {
        GA::BinaryRepresentation<NF> individual(n);
        double score = objective(individual);
        if (score < bestScore) {
            bestScore = score;
        }
    }
    return bestScore;
}

template<size_t NF>
double FacilityLocation::Solver<NF>::greedy(FacilityLocation::Instance<NF> &instance) {
    // This algorithm is based on the paper "A new greedy approach for facility location problems"
    // by K. Jain, M. Mahdian and A. Saberi

    // The value of budget increments is arbitrary, we choose here a bit less than
    // the smallest difference between two opening cost (enough to distinguish two facilities)
    double budgetQuantum = instance.cost(0);
    for (size_t iF1 = 1; iF1 < instance.numberFacility; ++iF1) {
        for (size_t iF2 = 0; iF2 < iF1; ++iF2) {
            if (std::abs(instance.cost(iF2) - instance.cost(iF1)) < budgetQuantum) {
                budgetQuantum = std::abs(instance.cost(iF2) - instance.cost(iF1));
            }
        }
    }
    budgetQuantum /= 2;

    // INITIALIZATION

    bool *connectedCustomer = new bool[instance.numberCustomer];
    size_t *connection = new size_t[instance.numberCustomer]; // Id of the facility to which the customer is connected
    double *budgetCustomer = new double[instance.numberCustomer];
    for (size_t iC = 0; iC < instance.numberCustomer; ++iC) {
        connectedCustomer[iC] = false;
        budgetCustomer[iC] = 0.;
    }

    bool *openedFacility = new bool[instance.numberFacility];
    for (size_t iF = 0; iF < instance.numberFacility; ++iF) {
        openedFacility[iF] = false;
    }

    // ITERATION
    do {
        // Increase budgets of unconnected customers
        for (size_t iC = 0; iC < instance.numberCustomer; ++iC) {
            if (!connectedCustomer[iC]) {
                budgetCustomer[iC] += budgetQuantum;
            }
        }

        // Updates connections
        for (size_t iF = 0; iF < instance.numberFacility; ++iF) {
            if (openedFacility[iF]) {
                for (size_t iC = 0; iC < instance.numberCustomer; ++iC) {
                    if (!connectedCustomer[iC] && budgetCustomer[iC] >= instance.distance(iF, iC)) {
                        // Connection of a customer to an already opened facility
                        connection[iC] = iF;
                        connectedCustomer[iC] = true;
                    }
                }
            } else {
                double budgetFacility = 0.;
                for (size_t iC = 0; iC < instance.numberCustomer; ++iC) {
                    if (connectedCustomer[iC] && instance.distance(connection[iC], iC) > instance.distance(iF, iC)) {
                        budgetFacility += instance.distance(connection[iC], iC) - instance.distance(iF, iC);
                    } else if (!connectedCustomer[iC] && budgetCustomer[iC] > instance.distance(iF, iC)) {
                        budgetFacility += budgetCustomer[iC] - instance.distance(iF, iC);
                    }
                }
                if (budgetFacility >= instance.cost(iF)) {
                    // Open a new facility and connect customers which had contributed
                    for (size_t iC = 0; iC < instance.numberCustomer; ++iC) {
                        if (connectedCustomer[iC] &&
                            instance.distance(connection[iC], iC) > instance.distance(iF, iC)) {
                            connection[iC] = iF;
                            connectedCustomer[iC] = true;
                        } else if (!connectedCustomer[iC] && budgetCustomer[iC] > instance.distance(iF, iC)) {
                            connection[iC] = iF;
                            connectedCustomer[iC] = true;
                        }
                    }
                    openedFacility[iF] = true;
                }
            }
        }

    } while (containsFalse(connectedCustomer, instance.numberCustomer));

    // COMPUTE SCORE
    double score = 0.;
    for (size_t iF = 0; iF < instance.numberFacility; ++iF) {
        if (openedFacility[iF]) {
            score += instance.cost(iF);
        }
    }
    for (size_t iC = 0; iC < instance.numberCustomer; ++iC) {
        score += instance.distance(connection[iC], iC);
    }

    return score;
}

template<size_t NF>
bool FacilityLocation::Solver<NF>::containsFalse(const bool *array, size_t size) {// Search for unconnected customers
    for (size_t i = 0; i < size; ++i) {
        if (!array[i]) {
            return true;
        }
    }
    return false;
}
