/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "sensor.h"

using namespace base;
using TestNum::eq;

namespace vg {

PUBLISH(Sensor)

Sensor::Sensor(QString name, Box *parent)
    : Box(name, parent)
{
    help("provides sensor inputs");
    Input(indoorsTemperatureIn).equals(missing);
    Input(indoorsRhIn).equals(missing);
    Input(indoorsCo2In).equals(missing);
    Input(indoorsWindspeedIn).equals(missing);
    Input(outdoorsTemperatureIn).equals(missing);
    Input(outdoorsRhIn).equals(missing);
    Input(outdoorsCo2In).equals(missing);
    Input(outdoorsGlobalRadiationIn).equals(missing);
    Input(outdoorsWindSpeedIn).equals(missing);
    Input(soilTemperatureIn).equals(missing);
    Input(sinb).imports("sun[sinb]");
    Output(indoorsTemperature);
    Output(indoorsRh);
    Output(indoorsAh);
    Output(indoorsCo2);
    Output(indoorsWindspeed);
    Output(outdoorsTemperature);
    Output(outdoorsRh);
    Output(outdoorsCo2);
    Output(outdoorsGlobalRadiation);
    Output(outdoorsWindSpeed);
    Output(soilTemperature);
}

void Sensor::reset() {
    update();
}

void Sensor::update() {
    // Indoors
    indoorsTemperature = isMissing(indoorsTemperatureIn) ? 20. : indoorsTemperatureIn;
    indoorsRh = (indoorsRhIn<=0.) ? 80. : indoorsRhIn;
    indoorsAh = phys_math::ahFromRh(indoorsTemperature, indoorsRh);
    indoorsCo2 = isMissing(indoorsCo2In) ? 800. : indoorsCo2In;
    indoorsWindspeed = isMissing(indoorsWindspeedIn) ? 0.1 : indoorsWindspeedIn;
    // Outdoors
    outdoorsTemperature = isMissing(outdoorsTemperatureIn) ? indoorsTemperature : outdoorsTemperatureIn;
    outdoorsRh = (outdoorsRhIn<=0.) ? indoorsRh : outdoorsRhIn;
    outdoorsCo2 = isMissing(outdoorsCo2In) ? 405. : outdoorsCo2In;
    outdoorsGlobalRadiation = isMissing(outdoorsGlobalRadiationIn) ?
                estimateGlobalRadiation() : outdoorsGlobalRadiationIn;
    outdoorsWindSpeed = isMissing(outdoorsWindSpeedIn) ? 4. : outdoorsWindSpeedIn;
    soilTemperature = isMissing(soilTemperatureIn) ? 10. : soilTemperatureIn;
}

bool Sensor::isMissing(double value) const {
    return eq(value, missing);
}

double Sensor::estimateGlobalRadiation() const {
    const double &T(outdoorsTemperature);
    double y = 0.1802 + 13.4796*sinb + 0.0102*T - 10.9866*sinb*sinb + 0.00043*T*T
            - 0.0598*sinb*T + 0.005115*sinb*sinb*T*T;
    y = y*y*y*y;
    return (y > 750.) ? 750 : y;

}

} //namespace

