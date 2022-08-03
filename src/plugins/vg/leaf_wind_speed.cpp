/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <stdlib.h>
#include "leaf_wind_speed.h"
#include <base/publish.h>

using std::max;
using namespace base;

namespace vg {

PUBLISH(LeafWindSpeed)

LeafWindSpeed::LeafWindSpeed(QString name, Box *parent)
    : Box(name, parent)
{
    help("computes leaf wind speed");
    Input(indoorsWindSpeedMinimum).equals(0.025).help("Minimum indoors wind speed [m/s]").unit("m/s");
    Input(indoorsWindSpeed).imports("indoors/windSpeed[value]",CA).unit("m/s");
    Output(value).help("Wind speed at leaf level").unit("m/s");
}

void LeafWindSpeed::reset() {
    value = indoorsWindSpeedMinimum;
}

void LeafWindSpeed::update() {
    value = max(indoorsWindSpeed, indoorsWindSpeedMinimum);
}


} //namespace

