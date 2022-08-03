/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Isaac Kwesi Abuley , Aarhus University [ikabuley at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "potato_development_we.h"

using namespace base;

namespace potato {

PUBLISH(PotatoDevelopmentWE)

PotatoDevelopmentWE::PotatoDevelopmentWE(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(T).imports("weather[Tavg]");
    Input(P).imports("calendar[dayLength]");
    Input(Tmin).equals(4.).help("Lower temperature threshold for development (oC)");
    Input(Topt).equals(15.).help("Optimum temperature for development (oC)");
    Input(Tmax).equals(28.).help("Upper temperature threshold for development (oC)");
    Input(Pcrit).equals(10.7).help("Critical day length [h]");
    Input(omega).equals(0.0645).help("Slope on day length response [h-1]");
    Input(rMaxVeg).equals(0.0793).help("Max. development rate in vegetative phase ([d-1]");
    Input(rMaxTub).equals(0.0260).help("Max. development rate in tuberisation phase ([d-1]");
    Input(rMaxSen).equals(0.0121).help("Max. development rate in senescence phase ([d-1]");
    Output(stepVeg).help("Development rate vegetative phase ([d-1]");
    Output(stepTub).help("Development rate tuberisation phase ([d-1]");
    Output(stepSen).help("Development rate senescence phase ([d-1]");
}

void PotatoDevelopmentWE::reset() {
    _alpha = log(2)/log((Tmax-Tmin)/(Topt-Tmin));
}

void PotatoDevelopmentWE::update() {
    stepVeg = rMaxVeg*fT()*fP();
    stepTub = rMaxTub*fT();
    stepSen = rMaxSen*fT();
}

double PotatoDevelopmentWE::fP() const {
    return (P < Pcrit) ? 1. : exp(-omega*(P-Pcrit));
}

double PotatoDevelopmentWE::fT() const {
    double y = 0;
    if (T>Tmin && T<Tmax) {
        double y1 = 2*p(T-Tmin) * p(Topt-Tmin) - p(T-Tmin,2),
               y2 = p(Topt-Tmin, 2);
        y = y1/y2;
     }
    return y;
}

double PotatoDevelopmentWE::p(double x, int n) const {
    return pow(x, n*_alpha);
}

}
