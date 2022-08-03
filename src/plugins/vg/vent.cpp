/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "vent.h"

using namespace base;

namespace vg {
	
PUBLISH(Vent)

Vent::Vent(QString name, Box *parent)
    : Box(name, parent)
{
    help("computes effective area of a set of vents");
    Input(length).imports("construction/geometry[length]",CA).unit("m");
    Input(width).equals(1).help("Width of one window (m)").unit("m");
    Input(number).equals(1).help("Number of windows").unit("-");
    Input(transmissivity).equals(1.).help("Air transmissivity through vent").unit("[0;1]");
    Output(area).help("Gross area").unit("m2");
    Output(effectiveArea).help("Effective area").unit("m2");
}

void Vent::reset() {
    update();
}

void Vent::update() {
    area = length*width*number;
    effectiveArea = area*transmissivity;
}

} //namespace


