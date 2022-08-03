/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Byju N. Govindan, University of Minnesota [ngbyju at umn.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
ï»¿#include <base/publish.h>
#include "schoolfield_dev_rate.h"

using namespace base;

namespace bmsb {

PUBLISH(SchoolFieldDevRate)

SchoolFieldDevRate::SchoolFieldDevRate(QString name, QObject *parent)
    : Box(name, parent)
{
    help("estimate the stage-specific daily developmental rate using SchoolField model; later replace daily rate to hourly rate");
        Input(TPhi).equals(298).help("Development rate at the intrinsic optimum temperature TÎ¦ (1/day) assuming no enzyme inactivation (days -1)");
        Input(dailytempC).equals(17).help("Absolute temperature (K) (237.16Â° K = 0Â°C)");
        Input(rhoPhi).equals(3.734e-01).help("Length of day (hours)");
        Input(HA).equals(3.990e+04).help("Enthalpy of activation of the reaction that is catalyzed by the enzyme (cal/mol)");
        Input(HL).equals(3.311e+04).help("Change in enthalpy associated with low-temperature inactivation of the enzyme (cal/mol)");
        Input(HH).equals(9.835e+05).help("Change in enthalpy associated with high-temperature inactivation of the enzyme (cal/mol)");
        Input(TL).equals(2.930e+02).help("Temperature at which the enzyme is Â½ active and Â½ low-temperature inactive (Kelvin)");
        Input(TH).equals(3.087e+02).help("Intrinsic optimum temperature at which the probability of enzyme being in the active state is maximal (K)");
        Input(R).equals(1.987).help("Gas constant (1.987 cal/deg/mol)");
        Output(numerator);
        Output(denominator);
        Output(devrate).help ("devrate refers to mean development rate (1/day)");
}

void SchoolFieldDevRate::reset() {
        update();
}

void SchoolFieldDevRate::update() {
    help("hourly temperature will later replace daily temp to estimate SchoolField dev rate");
    help("Note that input parameter supplied in degree Celsius for Temperature must be converted to in degree Kelvin");
//      Schoolfield model as in devRate packege in R: (rT ~ (p25 * (T + 273.16)/298 * exp(aa/1.987 * (1/298 - 1/(T +     273.16))))/(1 + exp(bb/1.987 * (1/cc - 1/(T + 273.16))) +     exp(dd/1.987 * (1/ee - 1/(T + 273.16)))))
//      p25 corresponds to rhoPhi, aa corresponds to HA, bb corresponds to HL, cc corresponds to TL, dd corresponds to HH and ee corresponds to TH (see; https://onlinelibrary.wiley.com/doi/full/10.1046/j.1420-9101.2001.00272.x)
        double dailytempK  = dailytempC + 273.16;
    	numerator   = ((rhoPhi) * (dailytempK/TPhi)) * (exp((HA/R) * ((1/TPhi)-(1/dailytempK))));
        denominator = (1 + (exp((HL/R) * ((1/TL) - (1/dailytempK)))) + (exp((HH/R) * ((1/TH) - (1/dailytempK)))));
        devrate     = numerator/denominator;
}


}

