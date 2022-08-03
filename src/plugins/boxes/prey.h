/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef PREY_H
#define PREY_H
#include <base/box.h>

namespace boxes {

class Prey : public base::Box
{
public: 
    Prey(QString name, Box *parent);
    void setFoodWebImports();
private:
    // Inputs
    double density, attackRate, gainFactor;
    // Outputs
    double loss, mortality;
};

} //namespace
#endif
