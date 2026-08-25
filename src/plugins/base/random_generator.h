/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef BASE_RANDOM_GENERATOR_H
#define BASE_RANDOM_GENERATOR_H

#include <memory>
#include <boost/random/mersenne_twister.hpp>

#include "base_dllexport.h"

namespace base {

class UNI_API RandomGenerator
{
public:
    RandomGenerator();
    typedef boost::mt19937 Generator;
    struct Shuffler {
        unsigned operator()(unsigned i);
    };
    Generator generator;
private: 
};

UNI_API RandomGenerator::Generator &randomGenerator();

UNI_API void seedRandomGenerator(int seed);

} //namespace
#endif
