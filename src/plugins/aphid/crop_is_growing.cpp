/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Stéphanie Saussure, Norwegian Institute of Bioeconomy Research [stephanie.saussure at nibio.no].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "crop_is_growing.h"

using namespace base;

namespace aphid {

PUBLISH(CropIsGrowing)

CropIsGrowing::CropIsGrowing(QString name, Box *parent)
    : Box(name, parent)
{
    help("computes if the crop has started to grow");
    Input(temperature).help("Daily average temperature").unit("oC");
    Input(T0).equals(5.).help("Threshold that triggers crop growth").unit("oC");
    Output(value);
}

void CropIsGrowing::reset() {
    temperature_2 =
    temperature_3 =
    temperature_4 =
    temperature_5 = 0.;
    value=false;
}

void CropIsGrowing::update() {
    if (!value) {
        value = (temperature>T0 && temperature_2>T0 && temperature_3>T0 && temperature_4>T0 && temperature_5>T0) ;
        temperature_5 = temperature_4;
        temperature_4 = temperature_3;
        temperature_3 = temperature_2;
        temperature_2 = temperature;
    }
}

}
