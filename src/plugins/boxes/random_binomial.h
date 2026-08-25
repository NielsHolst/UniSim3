/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef RANDOM_BINOMIAL_H
#define RANDOM_BINOMIAL_H

#include <base/base_dllexport.h>
#include "random_base_typed.h"

namespace boxes {

class UNI_API RandomBinomial : public RandomBaseTyped<bool>
{
public: 
    RandomBinomial(QString name, Box *parent);
private:
    void drawValue();
};

} //namespace
#endif
