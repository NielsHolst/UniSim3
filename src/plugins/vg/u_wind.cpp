/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "u_wind.h"

using namespace base;

namespace vg {

PUBLISH(UWind)

UWind::UWind(QString name, Box *parent)
    : Box(name, parent)
{
    help("calculates U-values of wind-exposed cover");
    Input(windSpeed).imports("outdoors[windSpeed]",CA).unit("m/s");
    Input(UwindMinimum).equals(2.8).help("Heat transfer coefficient at no wind").unit("W/m2/K");
    Input(UwindSlope).equals(1.2).help("Heat transfer coefficient linear increase with wind speed").unit("W/m2/K/(m/s)");
    Input(UwindExponent).equals(0.8).help("Reduces effect of high windspeed");
    Output(value).unit("W/K/m2 layer").help("U-value");
}

void UWind::reset() {
    update();
}

void UWind::update() {
    value = UwindMinimum + pow(windSpeed,UwindExponent)*UwindSlope;
}

} //namespace

