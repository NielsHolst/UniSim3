/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Byju N. Govindan, University of Minnesota [ngbyju at umn.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "ssi_dev_rate.h"

using namespace base;

namespace bmsb {

PUBLISH(SSIDevRate)

SSIDevRate::SSIDevRate(QString name, QObject *parent)
    : Box(name, parent)
{
    help("estimate the stage-specific daily developmental rate using SSI model; later replace daily rate to hourly rate");
        Input(TPhi).equals(12).help("Development rate at the intrinsic optimum temperature TÎ¦ (1/day) assuming no enzyme inactivation (days -1)");
        Input(dailytempC).equals(12).help("Absolute temperature (K) (237.15Â° K = 0Â°C)");
        Input(rhoPhi).equals(12).help("Length of day (hours)");
        Input(HA).equals(12).help("Enthalpy of activation of the reaction that is catalyzed by the enzyme (cal/mol)");
        Input(HL).equals(12).help("Change in enthalpy associated with low-temperature inactivation of the enzyme (cal/mol)");
        Input(HH).equals(12).help("Change in enthalpy associated with high-temperature inactivation of the enzyme (cal/mol)");
        Input(TL).equals(12).help("Temperature at which the enzyme is Â½ active and Â½ low-temperature inactive (Kelvin)");
        Input(TH).equals(12).help("Intrinsic optimum temperature at which the probability of enzyme being in the active state is maximal (K)");
        Input(R).equals(1.987).help("Gas constant (1.987 cal/deg/mol)");
        Output(numerator);
        Output(denominator);
        Output(devrate).help ("devrate refers to mean development rate (1/day)");
}

void SSIDevRate::reset() {
        update();
}

void SSIDevRate::update() {
    help("hourly temperature will later replace daily temp to estimate SSI rate");
    help("Note that input parameter supplied in degree Celsius for Temperature must be converted to in degree Kelvin");
        double dailytempK  = dailytempC + 273.15;
        numerator   = ((rhoPhi) * (dailytempK/TPhi)) * (exp( (HA/R) * ((1/TPhi)-(1/dailytempK))));
        denominator = (1 + (exp((HL/R) * ((1/TL) - (1/dailytempK)))) + (exp((HH/R) * ((1/TH) - (1/dailytempK)))));
        devrate     = numerator/denominator;
}


}
