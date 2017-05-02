#ifndef GENETICALGORITHM_ELITISMSELECTION_H
#define GENETICALGORITHM_ELITISMSELECTION_H

#include "GA/Selection.h"
#include "GA/Representation/BinaryRepresentation.h"

namespace GA {

    /**
     * TODO: documentation
     * @tparam Individual Type of individuals, must be a subclass of Representation
     */
    template<class Individual>
    class ElitismSelection : public Selection<Individual> {
        static_assert(std::is_base_of<Representation, Individual>::value,
                      "Individual not derived from Representation");

    public:
        using Population = typename Selection<Individual>::Population;

    public:
        ElitismSelection() = delete;

        ElitismSelection(const ElitismSelection&) = default;
        ElitismSelection(ElitismSelection&&) = default;
        ElitismSelection(double proportionSurvival);
        virtual ~ElitismSelection() = default;

        ElitismSelection &operator=(const ElitismSelection&) = default;
        ElitismSelection &operator=(ElitismSelection&&) = default;

        double getProportionSurvival() const;
        void setProportionSurvival(double proportionSurvival);

        Population operator()(const Population &population) override;

    protected:
        double proportionSurvival;

    };

}

#include "GA/Selection/ElitismSelection.tpp"

#endif //GENETICALGORITHM_ELITISMSELECTION_H
