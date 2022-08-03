/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include "arrhenius.h"

using namespace phys_math;

namespace vg {

inline double K(double T) {
    return T + T0;
}

inline double f(double DS, double DHd, double T) {
    return  1. + exp((DS*K(T) - DHd)/R/K(T));
}

double arrhenius(double v25, double Ea, double T) {
    return v25*exp(Ea*(T-25)/R/K(T)/K(25));
}

double arrhenius_peaked(double v25, double Ea, double DS, double DHd, double T) {
    return arrhenius(v25, Ea, T)*f(DS,DHd,25)/f(DS,DHd,T);
}

} //namespace

