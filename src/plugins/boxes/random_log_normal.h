/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef RANDOM_LOG_NORMAL_H
#define RANDOM_LOG_NORMAL_H
#include "random_base_typed.h"

namespace boxes {

class RandomLogNormal : public RandomBaseTyped<double>
{
public: 
    RandomLogNormal(QString name, Box *parent);
private:
    void checkInputs(QString text);
    void drawValue();
};

} //namespace
#endif
