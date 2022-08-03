/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/phys_math.h>
#include <base/test_num.h>
#include "heat_transfer_layer_base.h"

#include <base/dialog.h>

using namespace base;
using namespace phys_math;

namespace vg {

HeatTransferLayerBase::HeatTransferLayerBase(QString name, Box *parent)
    : HeatTransferLayerParameters(name, parent)
{
    help("holds the radiative parameters of a layer, e.g., cover or screen");
    // All m2 are m2 ground
    Input(parFluxDown).unit("umol/s/m2").help("PAR flux going down from bottom");
    Input(parFluxUp).unit("umol/s/m2").help("PAR flux going up from top");
    Input(swFluxDown).unit("W/m2").help("Short-wave flux going down from bottom");
    Input(swFluxUp).unit("W/m2").help("Short-wave flux going up from top");

    Input(convectiveInflux).help("Energy influx by convection").unit("W/m2");
    Input(conductiveInflux).help("Energy influx by conduction").unit("W/m2");
    Input(condensationRate).help("Water vapour condensation on layer").unit("kg/m2/s");
    Input(maxTemperatureRateOfChange).equals(100.).unit("K/min")
            .help("Max. rate of change in temperature; surplus goes to unusedInflux");
    Input(timeStep).imports("calendar[timeStepSecs]");

    Output(parAbsorbedFromAbove).unit("umol/s/m2").help("PAR flux absorbed from above");
    Output(parAbsorbedFromBelow).unit("umol/s/m2").help("PAR flux absorbed from below");
    Output(parAbsorbed).unit("umol/s/m2").help("PAR flux absorbed in total");
    Output(swAbsorbedFromAbove).unit("W/m2").help("Short-wave flux absorbed from above");
    Output(swAbsorbedFromBelow).unit("W/m2").help("Short-wave flux absorbed from below");
    Output(swAbsorbed).unit("W/m2").help("Short-wave flux absorbed in total");
    Output(lwAbsorbedFromAbove).unit("W/m2").help("Long-wave flux absorbed from above");
    Output(lwAbsorbedFromBelow).unit("W/m2").help("Long-wave flux absorbed from below");
    Output(lwAbsorbed).unit("W/m2").help("Long-wave flux absorbed in total ");
    Output(lwFluxDown).unit("W/m2").help("Long-wave flux going down from bottom");
    Output(lwFluxUp).unit("W/m2").help("Long-wave flux going up from");
    Output(absorbed).unit("W/m2").help("All flux (short+long) absorbed in total");
    Output(latentHeat).unit("W/m2").help("Latent heat from water condensation");
    Output(temperature).unit("oC").help("Surface temperature (average)");
    Output(temperatureTop).unit("oC").help("Surface temperature (top)");
    Output(temperatureBottom).unit("oC").help("Surface temperature (bottom)");
    Output(unusedInflux).unit("W/m2").help("Influx left over too keep within maxTemperatureRateOfChange");
}


void HeatTransferLayerBase::updateLwEmission() {
    double fluxDown = Sigma*p4K(temperatureBottom),
           fluxUp  = Sigma*p4K(temperatureTop);
    lwFluxDown = lwAbsorptivityBottom*fluxDown;
    lwFluxUp   = lwAbsorptivityTop*fluxUp;
}

void HeatTransferLayerBase::updateTemperature() {
    latentHeat = -condensationRate*LHe; // W/m2 = kg/m2/s * J/kg

    if (TestNum::neZero(heatCapacity)) {
//         maxRateOfChange = maxTemperatureRateOfChange/60., // K/s
        double rateOfChange  = (absorbed + convectiveInflux + conductiveInflux + latentHeat - lwFluxDown - lwFluxUp)/heatCapacity;  // K/s
//        if (rateOfChange > maxRateOfChange) {
//            unusedInflux = (rateOfChange - maxRateOfChange)*heatCapacity;  // W/m2 = K/s * J/K/m2
//            rateOfChange = maxRateOfChange;
//        }
//        else if (rateOfChange < -maxRateOfChange) {
//            unusedInflux = (rateOfChange + maxRateOfChange)*heatCapacity;  // W/m2 = K/s * J/K/m2
//            rateOfChange = -maxRateOfChange;
//        }
//        else {
//            unusedInflux = 0.;
//        }
        temperature += rateOfChange*timeStep;
    }
    temperatureTop = temperatureBottom = temperature;
}


} //namespace

