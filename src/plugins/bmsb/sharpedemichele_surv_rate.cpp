/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Byju N. Govindan, University of Minnesota [ngbyju at umn.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
ï»¿#include <base/publish.h>
#include "sharpedemichele_surv_rate.h"

using namespace base;

namespace bmsb {

PUBLISH(SharpeDeMicheleSurvRate)

SharpeDeMicheleSurvRate::SharpeDeMicheleSurvRate(QString name, QObject *parent)
    : Box(name, parent)
{
    help("estimate the stage-specific daily survival rate using SharpeDeMichele model; later replace daily rate to hourly rate");
        Input(dailytempC).equals(17).help("Temperature (in Â°C), must covert to Kelvin (237.16Â° K = 0Â°C)");
        Input(Phi).equals(9.889e+0).help("A coversion factor with no thermodyamic meaning");
        Input(HA).equals(6.404e+03).help("Enthalpy of activation of the reaction that is catalyzed by the enzyme (cal/mol)");
        Input(SL).equals(-7.920e+02).help("Change in entropy associated with low-temperature inactivation of the enzyme (cal/deg/mol)");
        Input(SH).equals(7.060e+02).help("Change in entropy associated with high-temperature inactivation of the enzyme (cal/deg/mol)");
        Input(HL).equals(-2.303e+05).help("Change in enthalpy associated with low-temperature inactivation of the enzyme (cal/mol)");
        Input(HH).equals(2.147e+05).help("Change in enthalpy associated with high-temperature inactivation of the enzyme (cal/mol)");
        Input(R).equals(1.987).help("Gas constant (1.987 cal/deg/mol)");
        Output(numerator);
        Output(denominator);
        Output(survrate).help ("survrate refers to mean development rate (1/day)");
}

void SharpeDeMicheleSurvRate::reset() {
        update();
}

void SharpeDeMicheleSurvRate::update() {
    help("hourly temperature will later replace daily temp to estimate SharpeDeMichele surv rate");
    help("Note that input parameter supplied in degree Celsius for Temperature must be converted to in degree Kelvin");
//      SharpeDeMichele model as in devRate packege in R: ( rT ~ ((T + 273.16) * exp((aa - bb/(T + 273.16))/1.987))/(1 + exp((cc - dd/(T + 273.16))/1.987) + exp((ff - gg/(T + 273.16))/1.987)))
//      aa corresponds to Phi, bb corresponds to HA, cc corresponds to SL, dd corresponds to HL, ff corresponds to SH and gg corresponds to HH
//      (see https://www.sciencedirect.com/science/article/pii/002251937790265X?via%3Dihub)
//      (See also, https://www-sciencedirect-com.ezp2.lib.umn.edu/science/article/pii/0022519381902460)
        double dailytempK  = dailytempC + 273.16;
        numerator = ((dailytempK) * exp((Phi - HA/(dailytempK))/R));
        denominator = (1 + exp((SL - HL/(dailytempK))/R) + exp((SH - HH/(dailytempK))/R));
        survrate     = numerator/denominator;
}


}



