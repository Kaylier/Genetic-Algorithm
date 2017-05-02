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
    class ProbabilistSelection : public Selection<Individual> {
        static_assert(std::is_base_of<Representation, Individual>::value,
                      "Individual not derived from Representation");

    public:
        using Population = typename Selection<Individual>::Population;

    public:
        ProbabilistSelection() = default;
        ProbabilistSelection(const ProbabilistSelection&) = default;
        ProbabilistSelection(ProbabilistSelection&&) = default;
        virtual ~ProbabilistSelection() = default;

        ProbabilistSelection &operator=(const ProbabilistSelection&) = default;
        ProbabilistSelection &operator=(ProbabilistSelection&&) = default;

        Population operator()(const Population &population) override;

    };

}

#include "GA/Selection/ProbabilistSelection.tpp"

#endif //GENETICALGORITHM_ELITISMSELECTION_H
