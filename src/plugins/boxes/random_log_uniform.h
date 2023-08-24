/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef RANDOM_LOG_UNIFORM_H
#define RANDOM_LOG_UNIFORM_H
#include "random_base_typed.h"

namespace boxes {

class RandomLogUniform : public RandomBaseTyped<double>
{
public: 
    RandomLogUniform(QString name, Box *parent);
    void checkInputs(QString text);
    void drawValue();
};

} //namespace
#endif
