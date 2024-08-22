/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "budget_layer.h"
#include "budget_volume.h"

using namespace base;
using namespace phys_math;

namespace vg {

PUBLISH(BudgetVolume)

BudgetVolume::BudgetVolume(QString name, base::Box *parent)
    : Box(name, parent)
{
    doWriteOnCommand(false);
    Input(initTemperature).equals(20.).unit("oC").help("Initial temperature");
    Input(initRh).equals(70.).unit("%").help("Initial relative humidity");
    Input(initCo2).equals(400.).unit("ppm").help("Initial CO2 concentration");
    Output(temperature).unit("oC").help("Temperature");
    Output(rh).unit("%").help("Relative humidity");
    Output(ah).unit("kg/m3").help("Absolute humidity");
    Output(co2).unit("ppm").help("CO2 concentration");
    Output(heatInflux).unit("W/m2").help("Convective heat influx");
}

void BudgetVolume::addLayer(const BudgetLayer *layer) {
    layers << layer;
}

void BudgetVolume::reset() {
    temperature = initTemperature;
    rh = initRh;
    ah = ahFromRh(temperature, rh);
    co2 = initCo2;
    updateRh();
}

// Updated by Budget!

void BudgetVolume::updateRh() {
    rh = rhFromAh(temperature, ah);
}

void BudgetVolume::addHeatInflux(double flux) {
    heatInflux += flux;
}

}

