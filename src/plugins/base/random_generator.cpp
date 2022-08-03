/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <boost/random.hpp>
#pragma GCC diagnostic pop

#include "random_generator.h"

namespace base {

std::unique_ptr<RandomGenerator> RandomGenerator::theRandomGenerator = nullptr;

RandomGenerator::RandomGenerator()
{
}

unsigned RandomGenerator::Shuffler::operator()(unsigned i) {
    boost::uniform_int<> rng(0, int(i) - 1);
    return unsigned ( rng(randomGenerator()) );
}


RandomGenerator::Generator &randomGenerator() {
    if (!RandomGenerator::theRandomGenerator)
        RandomGenerator::theRandomGenerator = std::unique_ptr<RandomGenerator>( new RandomGenerator );
    return (RandomGenerator::theRandomGenerator->generator);
}

void seedRandomGenerator(int seed) {
    static bool seeded = false;
    if (!seeded) {
        randomGenerator().seed(unsigned(seed));
        seeded = true;
    }
}

} // namespace
