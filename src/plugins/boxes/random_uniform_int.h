/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef RANDOM_UNIFORM_INT_H
#define RANDOM_UNIFORM_INT_H
#include <boost/random/uniform_01.hpp>
#include <boost/random/variate_generator.hpp>
#include <base/random_generator.h>
#include "random_base_typed.h"

namespace boxes {

class RandomUniformInt : public RandomBaseTyped<int>
{
public: 
    RandomUniformInt(QString name, Box *parent);
    void drawValue();
};

} //namespace
#endif
