/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Isaac Kwesi Abuley , Aarhus University [ikabuley at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef POTATO_DEVELOPMENT_WE_H
#define POTATO_DEVELOPMENT_WE_H
#include <base/box.h>

namespace potato {

class PotatoDevelopmentWE: public base::Box
{
public:
    PotatoDevelopmentWE(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double Tmin, Topt, Tmax,
           Pcrit, omega,
           rMaxVeg, rMaxTub, rMaxSen,
           T, P;
    // Outputs
    double stepVeg, stepTub, stepSen;
    // Data
    double _alpha;
    // Methods
    double fT() const;
    double fP() const;
    double p(double x, int n=1) const;
};

}
#endif
