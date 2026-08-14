/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "soil_water_thresholds.h"

using namespace base;

namespace coffee {

PUBLISH(SoilWaterThresholds)

SoilWaterThresholds::SoilWaterThresholds(QString name, QObject *parent)
    : Box(name, parent) {
    help("holds soil water parameters");
    Input(concWaterMax).equals(0.63).unit("m3/m3").help("Maximum soil water concentration");
    Input(propWaterDry).equals(0.01).unit("[0;1]").help("Proportional accessibility when dry");
    Input(propWaterWilt).equals(0.41).unit("[0;1]").help("Proportional accessibility at wilting point");
    Input(propWaterFieldCap).equals(0.65).unit("[0;1]").help("Proportional accessibility at field capacity");
    Input(propWaterSaturated).equals(0.87).unit("[0;1]").help("Proportional accessibility at saturation");
    Output(dry).unit("m3/m3").help("Accessible water when dry");
    Output(wilting).unit("m3/m3").help("Accessible water at wilting point");
    Output(fieldCap).unit("m3/m3").help("Accessible water at field capacity");
    Output(saturated).unit("m3/m3").help("Accessible water when saturated");
}

void SoilWaterThresholds::reset() {
    dry       = concWaterMax*propWaterDry;
    wilting    = concWaterMax*propWaterWilt;
    fieldCap  = concWaterMax*propWaterFieldCap;
    saturated = concWaterMax*propWaterSaturated;
}

} //namespace

