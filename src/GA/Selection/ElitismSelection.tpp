#include <cassert>

template<class Individual>
GA::ElitismSelection<Individual>::ElitismSelection(double proportionSurvival) {
    assert(0. < proportionSurvival && proportionSurvival < 1.);
    this->setProportionSurvival(proportionSurvival);
}

template<class Individual>
double GA::ElitismSelection<Individual>::getProportionSurvival() const {
    return proportionSurvival;
}

template<class Individual>
void GA::ElitismSelection<Individual>::setProportionSurvival(double proportionSurvival) {
    assert(0. < proportionSurvival && proportionSurvival <= 1.);
    ElitismSelection::proportionSurvival = proportionSurvival;
}

template<class Individual>
typename GA::ElitismSelection<Individual>::Population GA::ElitismSelection<Individual>::operator()
        (GA::ElitismSelection<Individual>::Population &population) {
    size_t countToSave = (size_t) ((double) population.size() * proportionSurvival);
    if (countToSave < 1) {
        countToSave = 1;
    }
    auto it = population.begin();
    for (; countToSave > 0; --countToSave) {
        it++;
    }
    population.erase(it, population.end());
    return population;
}

template<class Individual>
typename GA::ElitismSelection<Individual>::Population GA::ElitismSelection<Individual>::operator()
        (const GA::ElitismSelection<Individual>::Population &population) {
    Population out;
    size_t countToSave = (size_t) ((double) population.size() * proportionSurvival);
    if (countToSave < 1) {
        countToSave = 1;
    }
    auto it = population.begin();
    for (; countToSave > 0; --countToSave) {
        out.emplace(it->first, it->second);
        it++;
    }
    return out;
}
