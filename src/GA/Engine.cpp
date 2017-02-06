#include <cassert>
#include <cstdlib> // rand
#include <cmath> // sqrt

#include "GA/Engine.h"

template<class Individual>
GA::Engine<Individual>::Engine(Objective<Individual> &objective,
                               Crossover<Individual> &crossover,
                               Mutation<Individual> &mutation,
                               Selection<Individual> &selection) :
        objective(objective),
        crossover(crossover),
        mutation(mutation),
        selection(selection),
        populationSize(1),
        population() {}

template<class Individual>
GA::Objective<Individual> &GA::Engine<Individual>::getObjective() const {
    return this->objective;
}

template<class Individual>
GA::Crossover<Individual> &GA::Engine<Individual>::getCrossover() const {
    return this->crossover;
}

template<class Individual>
GA::Mutation<Individual> &GA::Engine<Individual>::getMutation() const {
    return this->mutation;
}

template<class Individual>
GA::Selection<Individual> &GA::Engine<Individual>::getSelection() const {
    return this->selection;
}

template<class Individual>
size_t GA::Engine<Individual>::getPopulationSize() const {
    return this->populationSize;
}

template<class Individual>
void GA::Engine<Individual>::setObjective(GA::Objective<Individual> &objective) {
    this->objective = objective;
}

template<class Individual>
void GA::Engine<Individual>::setCrossover(GA::Crossover<Individual> &crossover) {
    this->crossover = crossover;
}

template<class Individual>
void GA::Engine<Individual>::setMutation(GA::Mutation<Individual> &mutation) {
    this->mutation = mutation;
}

template<class Individual>
void GA::Engine<Individual>::setSelection(GA::Selection<Individual> &selection) {
    this->selection = selection;
}

template<class Individual>
void GA::Engine<Individual>::setPopulationSize(size_t populationSize) {
    assert(populationSize != 0);
    this->populationSize = populationSize;
}

template<class Individual>
void GA::Engine<Individual>::initialize() {
    population.clear();
    Individual individual;
    for (size_t i = populationSize; i != 0; --i) {
        individual.randomize();
        population.emplace(objective(individual), individual);
    }
}

template<class Individual>
void GA::Engine<Individual>::initialize(size_t populationSize) {
    assert(populationSize != 0);
    this->setPopulationSize(populationSize);
    return this->initialize();
}

template<class Individual>
double GA::Engine<Individual>::step() {
    return this->step(1);
}

template<class Individual>
double GA::Engine<Individual>::step(unsigned int numberStep) {
    typename Population::iterator it1, it2;
    Individual individual;

    for (unsigned int i = numberStep; i != 0; --i) {

        population = selection(population);

        while (population.size() < populationSize) {
            it1 = population.begin();
            it2 = population.begin();
            std::advance(it1, rand() % population.size());
            std::advance(it2, rand() % population.size());

            individual = std::move(mutation(crossover(it1->second, it2->second)));

            population.emplace(objective(individual), individual);
        }
    }
    return population.cbegin()->first;
}

template<class Individual>
double GA::Engine<Individual>::getScore() const {
    return population.cbegin()->first;
}

template<class Individual>
const Individual GA::Engine<Individual>::getBest() const {
    return population.cbegin()->second;
}

template<class Individual>
double GA::Engine<Individual>::getMean() const {
    return this->getMean(population.size());
}

template<class Individual>
double GA::Engine<Individual>::getMean(size_t count) const {
    assert(count <= population.size());
    double total = 0.;
    auto it = population.cbegin();
    for (int i = 0; i < count; ++i) {
        total += it->first;
        it++;
    }
    return total / count;
}

template<class Individual>
double GA::Engine<Individual>::getStandardDeviation() const {
    return getStandardDeviation(population.size());
}

template<class Individual>
double GA::Engine<Individual>::getStandardDeviation(size_t count) const {
    assert(count <= population.size());
    double mean = 0.;
    double meanOfSquares = 0.;
    auto it = population.cbegin();
    for (size_t i = 0; i < count; ++i) {
        meanOfSquares += it->first * it->first;
        mean += it->first;
        it++;
    }
    meanOfSquares /= count;
    mean /= count;
    return std::sqrt(meanOfSquares - mean*mean);
}

