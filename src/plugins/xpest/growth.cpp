/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "damage.h"
#include "growth.h"
#include <base/publish.h>

using std::min;
using namespace base;

namespace xpest {

PUBLISH(Growth)
	
Growth::Growth(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(radiation).imports("weather[irradiationMJ]");
    Input(lai).equals(2.).help("Crop leaf area index");
    Input(k).equals(0.65).help("Crop Extinction coefficient");
    Input(rue).equals(1.19).help("Radiation use efficieny").unit("g/MJ");

    Input(assimilateDemands).imports("damages/assimilateDemand/*");
    Input(assimilateDiversions).imports("damages/assimilateDiversion/*");
    Input(laiReductions).imports("damages/lai/*");
    Input(rueReductions).imports("damages/rue/*");

    Output(production).help("Amount of assimilate produced").unit("g/m2/day");
    Output(loss).help("Amount of assimilate lost to pests").unit("g/m2/day");
    Output(gain).help("Net gain of assimilate = production - loss").unit("g/m2/day");
    Output(rueReduction).help("Reduction in RUE").unit("[0;1]");
    Output(laiReduction).help("Reduction in affective LAI").unit("[0;1]");
}

void Growth::update() {
//    rate = radiation*rue*(1. - exp(-k*lai));
    setRueReduction();
    setLaiReduction();
    production = radiation*rue*(1.-rueReduction)*(1. - exp(-k*lai*(1.-laiReduction)));
    loss = production*assimilateReduction(production);
    gain = production - loss;
}

namespace {
    double demandFactor(const QVector<double> &demands, double production) {
        double factor = 1.;
        if (production > 0.) {
            for (double demand : demands) {
                double diversion = min(demand/production, 1.);
                factor *= 1. - diversion;
            }
        }
        return factor;
    }

    double diversionFactor(const QVector<double> &diversions) {
        double factor = 1.;
        for (double diversion : diversions) {
            factor *= 1. - diversion;
        }
        return factor;
    }
}

void Growth::setRueReduction() {
    rueReduction = 1. - diversionFactor(rueReductions);
}

void Growth::setLaiReduction() {
    laiReduction = 1. - diversionFactor(laiReductions);
}

double Growth::assimilateReduction(double production) {
    return 1. -
           demandFactor(assimilateDemands, production) * diversionFactor(assimilateDiversions);
}

} //namespace

