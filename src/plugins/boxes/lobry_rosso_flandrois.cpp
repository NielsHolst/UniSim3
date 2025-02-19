/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/exception.h>
#include <base/phys_math.h>
#include <base/publish.h>
#include "lobry_rosso_flandrois.h"

using namespace base;
using phys_math::sqr;

namespace boxes {

PUBLISH(LobryRossoFlandrois)

LobryRossoFlandrois::LobryRossoFlandrois(QString name, Box *parent)
    : PhysiologicalTime(name, parent) {
    help("computes non-linear development rate per day");
    Input(Tmin).unit("oC").help("Lower temperature threshold");
    Input(Topt).unit("oC").help("Optimal temperature");
    Input(Tmax).unit("oC").help("Upper temperature threshold");
    Input(maximum).unit("/oC").help("Maximum development rate");
}

void LobryRossoFlandrois::reset() {
    if (Tmin >= Tmax) {
        QString msg{"T0 (%1) must be smaller than Tmax (%2)"};
        ThrowException(msg.arg(Tmin).arg(Tmax)).context(this);
    }
    if (!(Tmin < Topt && Tmin < Tmax)) {
        QString msg{"Topt (%1) must be between T0 (%2) and Tmax (%3)"};
        ThrowException(msg.arg(Topt).arg(Tmin).arg(Tmax)).context(this);
    }
    _doMirror = (Topt < (Tmin + Tmax)/2.);
}

inline double f(double T, double Tmin, double Topt, double Tmax) {
    const double a = (T-Tmax)*sqr(T-Tmin)/(Topt-Tmin),
                 b = (Topt-Tmin)*(T-Topt) - (Topt-Tmax)*(Topt+Tmin - 2*T);
    return a/b;
}

void LobryRossoFlandrois::updateStep() {
    if (!isTicking)
        step = 0.;
    else if (T <= Tmin)
        step = 0.;
    else if (T < Tmax)
        step = maximum*(_doMirror ? f(mirror(T), Tmin, mirror(Topt), Tmax) : f(T, Tmin, Topt, Tmax));
    else
        step = 0;
    step *= timeStepDays;
}

} //namespace

