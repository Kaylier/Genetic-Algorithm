#ifndef GENETICALGORITHM_REPRESENTATION_H
#define GENETICALGORITHM_REPRESENTATION_H

namespace GA {

    class Representation {

    public:
        Representation() = default;
        Representation(const Representation&) = default;
        Representation(Representation&&) = default;
        virtual ~Representation() = 0;

        Representation &operator=(const Representation&) = default;
        Representation &operator=(Representation&&) = default;

        virtual void randomize() = 0;
    };

    inline Representation::~Representation() {}

}

#endif //GENETICALGORITHM_REPRESENTATION_H
