/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef RAIN_INTERCEPTION_H
#define RAIN_INTERCEPTION_H
#include <base/box.h>

namespace coffee {

class RainInterception : public base::Box
{
public: 
    RainInterception(QString name, QObject *parent);
    void update();
private:
    // Inputs
    double interceptionCoef,
        rainfall, laiCoffee, laiTree, timeStep;
    // Outputs
    double coffee, tree, total;
};

} //namespace
#endif
