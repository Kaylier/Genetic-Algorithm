#include <cassert>
#include <random>

template<class Individual>
typename GA::ProbabilistSelection<Individual>::Population GA::ProbabilistSelection<Individual>::operator()
        (const GA::ProbabilistSelection<Individual>::Population &population) {
    std::random_device rndDevice;
    std::default_random_engine rnd(rndDevice());
    std::uniform_real_distribution<double> distrib(0., 1.);

    Population out;
    auto it = population.begin();
    double size = (double) population.size();
    for (size_t i = 0; i < population.size(); i++) {
        if (distrib(rnd) > (double) i / size) {
            out.emplace(it->first, it->second);
        }
        ++it;
    }
    return out;
}
