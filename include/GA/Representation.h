#ifndef GENETICALGORITHM_REPRESENTATION_H
#define GENETICALGORITHM_REPRESENTATION_H

namespace GA {

    /**
     * Interface of a software representation of an individual.
     * Subclasses are intended to be a mask of specialization of required functor
     * of the genetic algorithm (many of them needs to know the internal structure
     * to do their job).
     * An required feature must be implemented: randomize()
     */
    class Representation {

    public:
        Representation() = default;
        Representation(const Representation&) = default;
        Representation(Representation&&) = default;
        virtual ~Representation() = 0;

        Representation &operator=(const Representation&) = default;
        Representation &operator=(Representation&&) = default;

        /**
         * Erases the information contained in the individual.
         * A call to this function will generate a new random individual
         * independent of its previous state.
         */
        virtual void randomize() = 0;
    };

    inline Representation::~Representation() {}

}

#endif //GENETICALGORITHM_REPRESENTATION_H
