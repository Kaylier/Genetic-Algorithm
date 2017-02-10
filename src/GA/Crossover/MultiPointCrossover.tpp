#include <queue>
#include <algorithm>

#include "GA/Crossover/MultiPointCrossover.h"

template<size_t N>
GA::MultiPointCrossover<GA::BinaryRepresentation<N>>::MultiPointCrossover(const unsigned int numberPoint) :
    numberPoint(numberPoint) {
    std::random_device rndDevice;
    this->rnd.seed(rndDevice());
}

template<size_t N>
GA::BinaryRepresentation<N>
GA::MultiPointCrossover<GA::BinaryRepresentation<N>>::operator()(const GA::BinaryRepresentation<N> &individual1,
                                                               const GA::BinaryRepresentation<N> &individual2) {
    std::uniform_int_distribution<size_t> distrib(0, N-1);
    BinaryRepresentation<N> child;
    std::vector<size_t> points(numberPoint);
    for (unsigned int i = 0; i < numberPoint; ++i) {
        points[i] = distrib(rnd);
    }
    std::sort(points.begin(), points.end());
    points.push_back(N);

    size_t n = 0;
    bool chooseIndividual1 = true;
    auto it = points.begin();
    while (it != points.end()) {
        while (n < *it) {
            child.set(n, chooseIndividual1? individual1[n] : individual2[n]);
            n++;
        }
        chooseIndividual1 = !chooseIndividual1;
        ++it;
    }
    return child;
}
