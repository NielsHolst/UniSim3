/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <stdlib.h>
#include "indoors_wind_speed.h"
#include <base/publish.h>

using namespace std;
using namespace base;

namespace vg {

PUBLISH(IndoorsWindSpeed)

IndoorsWindSpeed::IndoorsWindSpeed(QString name, Box *parent)
	: Box(name, parent)
{
    help("models indoors wind speed");
    Input(ventilation).imports("indoors/ventilation[flux]",CA).unit("m3/m3/h");
    Input(averageHeight).imports("geometry[averageHeight]",CA).unit("m");
    Input(minimumValue).equals(0.1).help("Lower bound on value").unit("m/s");
    Output(value).help("Indoors wind speed [m/s]").unit("m/s");
}

void IndoorsWindSpeed::reset() {
    value = minimumValue;
}

void IndoorsWindSpeed::update() {
    value = max(ventilation*averageHeight/3600., minimumValue); // m/s = h-1 * m *  h/s
}

} //namespace

