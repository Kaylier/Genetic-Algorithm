#ifndef FACILITYLOCATION_INSTANCE_H
#define FACILITYLOCATION_INSTANCE_H

#include <algorithm> // swap
#include <cstdlib> // size_t

namespace FacilityLocation {

    /**
     * An instance of a Facility Location Problem.
     * This only represent an static instance of the problem.
     * Static functions can be called to generate a new instance
     * <p>
     * The template NF fix the number of facilities.
     */
    template<size_t NF>
    class Instance {

    public:
        constexpr static size_t numberFacility = NF;
        const size_t numberCustomer;

        static Instance randomInstance(size_t numberCustomer);
        static Instance randomMetricInstance(size_t numberCustomer);
        static Instance load(std::string filename);

        Instance() = delete;
        Instance(const Instance<NF> &instance);
        Instance(Instance &&instance);
        ~Instance();

        Instance &operator=(Instance<NF> instance);
        Instance &operator=(Instance &&instance);
        template<size_t NF2>
        friend void swap(Instance<NF2> &first, Instance<NF2> &second);

        size_t getNumberFacility() const;
        size_t getNumberCustomer() const;
        double distance(size_t facility, size_t customer) const;
        double cost(size_t facility) const;

        void save(std::string filename) const;

    private:
        Instance(size_t numberCustomer);

        double *distances[NF];
        double openingCost[NF];

    };

    template<size_t NF>
    void swap(Instance<NF> &first, Instance<NF> &second);

}

#include "FacilityLocation/Instance.tpp"

#endif //FACILITYLOCATION_INSTANCE_H
