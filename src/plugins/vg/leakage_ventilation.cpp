/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/publish.h>
#include "leakage_ventilation.h"

using namespace base;

namespace vg {

PUBLISH(LeakageVentilation)

LeakageVentilation::LeakageVentilation(QString name, Box *parent)
	: Box(name, parent)
{
    help("computes air flux by leakage");
    Input(leakage).equals(1).help("Infiltration rate at a wind speed of 4 m/s").unit("/h");
    Input(windSpeed).imports("outdoors[windSpeed]", CA);
    Output(value).help("Air flux by leakage").unit("/h");
}

void LeakageVentilation::reset() {
    update();
}

void LeakageVentilation::update() {
    value = leakage*windSpeed/4.;
}

} //namespace

