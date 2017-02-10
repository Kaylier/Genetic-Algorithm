#ifndef GENETICALGORITHM_ENGINE_H
#define GENETICALGORITHM_ENGINE_H

#include <type_traits> // is_base_of
#include <random>

#include <map>

#include "GA/Crossover.h"
#include "GA/Mutation.h"
#include "GA/Objective.h"
#include "GA/Representation.h"
#include "GA/Selection.h"

#include "GA/Representation/BinaryRepresentation.h"

namespace GA {

    /**
     * This class is the core of genetic algorithms.
     * @tparam Individual Type of individuals, must be a subclass of Representation
     */
    template<class Individual>
    class Engine final {
        static_assert(std::is_base_of<Representation, Individual>::value,
                      "Individual not derived from Representation");

    private:
        /**
         * A population, or a set of individuals.
         * To simplify algorithms, the sorting of best individuals is managed by the
         * container during insertion.
         *
         * The required properties of the container are:
         * <ul>
         * <li>Contains pairs of Individual and double
         * <li>Can contain multiple identical pair
         * <li>Ordered after the values of doubles
         * </ul>
         */
        using Population = std::multimap<double, Individual>;

    public:
        Engine() = delete;

        Engine(const Engine &) = delete;

        Engine(Engine &&) = delete;

        Engine &operator=(const Engine &) = delete;

        Engine &operator=(Engine &&) = delete;

        /**
         * Constructor of engine.
         * More information about parameters can be found in respective classes.
         * @param objective The objective functor
         * @param crossover The crossover functor
         * @param mutation The mutation functor
         * @param selection The selection functor
         * @see Objective
         * @see Crossover
         * @see Mutation
         * @see Selection
         */
        Engine(Objective<Individual> &objective,
               Crossover<Individual> &crossover,
               Mutation<Individual> &mutation,
               Selection<Individual> &selection);

        /**
         * Default destructor
         */
        ~Engine() = default;

        /**
         * @return The objective functor bound to the engine
         * @see setObjective(Objective&)
         */
        Objective<Individual> &getObjective() const;

        /**
         * @return The crossover functor bound to the engine
         * @see setCrossover(Crossover&)
         */
        Crossover<Individual> &getCrossover() const;

        /**
         * @return The mutation functor bound to the engine
         * @see setMutation(Mutation&)
         */
        Mutation<Individual> &getMutation() const;

        /**
         * @return The selection functor bound to the engine
         * @see setSelection(Selection&)
         */
        Selection<Individual> &getSelection() const;

        /**
         * @return The aimed population size after the next step
         * @see setPopulationSize(size_t)
         */
        size_t getPopulationSize() const;

        /**
         * Set a new objective functor
         * @param objective The new functor
         * @see getObjective()
         */
        void setObjective(Objective<Individual> &objective);

        /**
         * Set a new crossover functor
         * @param crossover The new functor
         * @see getCrossover()
         */
        void setCrossover(Crossover<Individual> &crossover);

        /**
         * Set a new mutation functor
         * @param mutation The new functor
         * @see getMutation()
         */
        void setMutation(Mutation<Individual> &mutation);

        /**
         * Set a new selection functor
         * @param selection The new functor
         * @see getSelection()
         */
        void setSelection(Selection<Individual> &selection);

        /**
         * Set the size of population to aim after the next step.
         * The new size must be a strictly positive number.
         * @param populationSize The new population size
         * @see getPopulationSize()
         */
        void setPopulationSize(size_t populationSize);

        /**
         * Initialize a basic population composed of random individuals.
         * The size of the population is 1 by default, but can (and should) be modified
         * before the call to this function with setPopulationSize(size_t).
         * Each individual is generate via the function GA::Representation::randomize()
         * @see setPopulationSize(size_t)
         * @see initialize(size_t)
         */
        void initialize();

        /**
         * Initialize a basic population composed of random individuals.
         * The size of the population is defined by the parameter. Note that this
         * size will be kept after the call, as if setPopulationSize(size_t) had been
         * called.
         * Each individual is generate via the function GA::Representation::randomize()
         * @see setPopulationSize(size_t)
         * @see initialize()
         */
        void initialize(size_t populationSize);

        /**
         * Shortcut of step(1)
         * @return The score of the best individual generated
         * @see step(unsigned int)
         */
        double step();

        /**
         * Updates the population after an iteration of the genetic algorithm.
         * The size of the new population can be set with the function
         * setPopulationSize(size_t)
         * Please refers to the implementation for more details.
         * @param numberStep The number of step
         * @return The score of the best individual generated
         * @see setPopulationSize(size_t)
         */
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
         * Compute the mean of the scores of bests individuals of the population
         * @param count The number of individual considered
         * @return The mean score
         */
        double getMean(size_t count) const;

        /**
         * @return The standard deviation of the scores of the current population
         */
        double getStandardDeviation() const;

        /**
         * Compute the standard deviation of the scores of bests individuals
         * of the population
         * @param count The number of individual considered
         * @return The standard deviation
         */
        double getStandardDeviation(size_t count) const;

    private:
        std::default_random_engine rnd;

        Objective<Individual> &objective; /**< Bounded objective functor */
        Crossover<Individual> &crossover; /**< Bounded crossover functor */
        Mutation<Individual> &mutation; /**< Bounded mutation functor */
        Selection<Individual> &selection; /**< Bounded selection functor */

        size_t populationSize; /**< The aimed population size after the next step */

        Population population; /**< Current population */

    };

}

#include "GA/Engine.tpp"

#endif //GENETICALGORITHM_ENGINE_H
