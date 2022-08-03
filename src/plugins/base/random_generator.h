/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_RANDOM_GENERATOR_H
#define BASE_RANDOM_GENERATOR_H
#include <memory>
#include <boost/random/mersenne_twister.hpp>

namespace base {

class RandomGenerator
{
public: 
    typedef boost::mt19937 Generator;
    friend RandomGenerator::Generator &randomGenerator();
    struct Shuffler {
        unsigned operator()(unsigned i);
    };
private:
    RandomGenerator();
    Generator generator;
    static std::unique_ptr<RandomGenerator> theRandomGenerator;
};

RandomGenerator::Generator &randomGenerator();

void seedRandomGenerator(int seed);

} //namespace
#endif
