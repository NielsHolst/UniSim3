/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef PET_H
#define PET_H
#include <base/box.h>

namespace coffee {

class Pet : public base::Box
{
public: 
    Pet(QString name, QObject *parent);
    void update();
private:
    // Inputs
    double temperature, vp, globRad, windSpeed,
        kCoffee, kTree,
        lai, interceptedRain;
    // Outputs
    double potEvaporation, potTranspiration;
};

} //namespace
#endif
