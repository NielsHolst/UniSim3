/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "budget_layer.h"
#include "budget_volume.h"

using namespace base;

namespace vg {


BudgetVolume::BudgetVolume(QString name, base::Box *parent)
    : Box(name, parent)
{
    Input(initTemperature).equals(20.).unit("oC").help("Initial temperature");
    Input(initRh).equals(70.).unit("%").help("Initial relative humidity");
    Input(initCo2).equals(400.).unit("ppm").help("Initial CO2 concentration");
    Output(temperature).unit("oC").help("Temperature");
    Output(rh).unit("%").help("Relative humidity");
    Output(co2).unit("ppm").help("CO2 concentration");
    Output(heatInflux).unit("W/m2").help("Total convective/conductive heat influx");
}

void BudgetVolume::addLayer(const BudgetLayer *layer) {
    layers << layer;
}

void BudgetVolume::reset() {
    temperature = initTemperature;
    rh = initRh;
    co2 = initCo2;
    update();
}

void BudgetVolume::addHeatInflux(double flux) {
    heatInflux += flux;
}

}

