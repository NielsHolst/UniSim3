/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef RANDOM_UNIFORM_H
#define RANDOM_UNIFORM_H
#include "random_base_typed.h"

namespace boxes {

class RandomUniform : public RandomBaseTyped<double>
{
public: 
    RandomUniform(QString name, Box *parent);
    void drawValue();
};

} //namespace
#endif
