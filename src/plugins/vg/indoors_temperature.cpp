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
#include "indoors_temperature.h"

#include <base/dialog.h>

using namespace base;
using namespace phys_math;

//#define LOG(x) dialog().information(QString("IndoorsTemperature ") + #x + " " + QString::number(x));
#define LOG(x)

namespace vg {

PUBLISH(IndoorsTemperature)

IndoorsTemperature::IndoorsTemperature(QString name, Box *parent)
    : Box(name, parent)
{
    help("models indoors temperature");
    Input(initTemperature).equals(20.).help("Initial air temperature").unit("oC");
    Input(convectiveInflux).help("Energy influx by convection").unit("W/m2 ground");
    Input(timeStep).imports("calendar[timeStepSecs]");
    Input(groundArea).imports("geometry[groundArea]", CA);
    Input(volume).imports("geometry[volume]", CA);
    Input(airInflux).imports("indoors/ventilation[flux]", CA);
    Input(outdoorsTemperature).imports("outdoors[temperature]", CA);
    Input(keepConstant).equals(false).help("Keep temperature fixed at 'initTemperature'?");
    Input(scanTemperatures).equals(false).help("Scan a range of temperatures?");
    Output(value).help("Current air temperature").unit("oC");
    Output(advectiveEnergyFlux).help("Energy exchanged with outdoors by air exchange").unit("W/m2");
}

void IndoorsTemperature::reset() {
    _tick = 0;
    value = scanTemperatures ? scannedTemperature() : initTemperature;
}

inline double y(double y0, double z, double v, double dt) {
  return z + (y0 - z)*exp(-v*dt);
}

void IndoorsTemperature::update() {
    if (scanTemperatures) {
        ++_tick;
        value = scannedTemperature();
    }
    if (keepConstant)
        return;
    LOG(convectiveInflux);
    // First, adjust air temperature for convective heat
    double mass = RhoAir*volume,  // kg = kg/m3 * m3
           heatCapacity = CpAir*mass/groundArea,  // J/K/m2 = (J/kg/K) * kg / m2
           rateOfChange  = convectiveInflux/heatCapacity;  // K/s = W/m2 * K*m2/J
    value += rateOfChange*timeStep;

    // Then, adjust air temperature for advective heat
    double indoorsTemperatureFinal = y(value, outdoorsTemperature, airInflux, timeStep/3600.),
           dTemp = indoorsTemperatureFinal - value;
    LOG(dTemp);
    value = indoorsTemperatureFinal;

    // Compute temperature by air exchange in terms of energy flux
    // W/m2 = K * J/K/m2 /s
    advectiveEnergyFlux = dTemp*heatCapacity/timeStep;
}

double IndoorsTemperature::getTemperature() const {
    return value;
}

void IndoorsTemperature::setTemperature(double newValue) {
    if (!keepConstant)
        value = newValue;
}

double IndoorsTemperature::scannedTemperature() const {
    // With sim[step]==59, offset will become
    // 0, 5, 4, 3, 2, 1, -5, -4, -3, -2, -1, 0
    // Starts at 0 to set initial ah
    // Ends at 0 to achieve final values corresponding to 0 scenario
    const int j = _tick/5;
    int k;
    if (j==0)
      k = 0;
    else if (j<6)
      k = 6-j;
    else if (j<11)
      k = j-11;
    else
      k = 0;
    return initTemperature - k;
    /*
    for (i in 0:59) {
      j = floor(i/5)
      if (j==0) {
        k = 0
      } else if (j<6) {
        k = 6-j
      }
      else if (j<11) {
        k = j-11
      }
      else {
        k = 0
      }
      print(k)
    }
    */
}

} //namespace

