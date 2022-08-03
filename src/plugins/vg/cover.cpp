/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/environment.h>
#include <base/publish.h>
#include "cover.h"

using namespace base;

namespace vg {

PUBLISH(Cover)

Cover::Cover(QString name, Box *parent)
    : HeatTransferLayerParameters(name, parent)
{
    help("computes cover light and heat characteristics");
    Input(windSpeed).imports("outdoors[windSpeed]",CA).unit("m/s");
    Input(haze).equals(1.).help("Proportion of direct light transmitted as diffuse light").unit("[0;1]");
    Input(UwindMinimum).equals(2.8).help("Heat transfer coefficient at no wind").unit("W/m2/K");
    Input(UwindSlope).equals(1.2).help("Heat transfer coefficient linear increase with wind speed").unit("W/m2/K/(m/s)");
    Input(UwindExponent).equals(0.8).help("Reduces effect of high windspeed");
    port("area")->imports("../area[value]",CA);
    port("Ubottom")->equals(3.3);
    port("heatCapacity")->equals(8400.);
    /* density of glass       = 2.5kg per m2 per mm of thickness => 10 kg glass/m2
     * heat capacity of glass = 0.84 J/g/K = 840 J/kg/K = 8400 J/m2/K
    */
}

void Cover::reset() {
    update();
}

void Cover::update() {
    updateAbsorptivities();
    Utop = UwindMinimum + pow(windSpeed,UwindExponent)*UwindSlope;
}

} //namespace

