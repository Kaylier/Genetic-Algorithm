#ifndef GENETICALGORITHM_ENGINE_H
#define GENETICALGORITHM_ENGINE_H

#include <type_traits> // is_base_of

#include <map>

#include "GA/Crossover.h"
#include "GA/Objective.h"
#include "GA/Mutation.h"
#include "GA/Representation.h"
#include "GA/Selection.h"

#include "GA/Individual/BinaryRepresentation.h"

namespace GA {

    template<class Individual>
    class Engine final {
        static_assert(std::is_base_of<Representation, Individual>::value,
                      "Individual not derived from Representation");

    public:
        using Population = std::multimap<double, Individual>;

    public:
        Engine() = delete;
        Engine(const Engine&) = delete;
        Engine(Engine&&) = delete;
        Engine(Objective<Individual> &objective,
               Crossover<Individual>  &crossover,
               Mutation<Individual>  &mutation,
               Selection<Individual>  &selection);
        ~Engine() = default;

        Engine &operator=(const Engine&) = delete;
        Engine &operator=(Engine&&) = delete;

        Objective<Individual>  &getObjective() const;
        Crossover<Individual>  &getCrossover() const;
        Mutation<Individual>  &getMutation() const;
        Selection<Individual>  &getSelection() const;
        size_t getPopulationSize() const;
        void setObjective(Objective<Individual>  &objective);
        void setCrossover(Crossover<Individual>  &crossover);
        void setMutation(Mutation<Individual>  &mutation);
        void setSelection(Selection<Individual>  &selection);
        void setPopulationSize(size_t populationSize);

        void initialize();
        void initialize(size_t populationSize);
        double step();
        double step(unsigned int numberStep);

        /**
         * @return The score of the best individual of the current population
         */
        double getScore() const;
        /**
         * @return The best individual of the current population
         */
        const Individual getBest() const;
        /**
         * Compute the mean of the scores of the population.
         * @return The mean of the scores of the current population
         */
        double getMean() const;
        /**
         * Returns the mean of the scores of bests individuals of the population
         * @param count The number of individual considered
         * @return The mean score
         */
        double getMean(unsigned long count) const;
        /**
         * @return The standard deviation of the scores of the current population
         */
        double getStandardDeviation() const;
        /**
         * Returns the standard deviation of the scores of bests individuals
         * of the population
         * @param count The number of individual considered
         * @return The standard deviation
         */
        double getStandardDeviation(unsigned int count) const;

    private:
        Objective<Individual>  &objective;
        Crossover<Individual>  &crossover;
        Mutation<Individual>  &mutation;
        Selection<Individual>  &selection;

        size_t populationSize;

        Population population;

    };

    template class Engine<BinaryRepresentation<4>>;

}

#endif //GENETICALGORITHM_ENGINE_H
