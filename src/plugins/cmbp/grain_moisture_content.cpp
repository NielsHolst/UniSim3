/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** William Meikle, USDA [william.meikle at usda.gov].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "grain_moisture_content.h"

using namespace base;

namespace cmbp {

PUBLISH(GrainMoistureContent)

GrainMoistureContent::GrainMoistureContent(QString name, Box *parent)
    : Box(name, parent)
{
    Input(c1).help("Regression parameter").equals(0.523);
    Input(c2).help("Regression parameter").equals(-0.00967);
    Input(c3).help("Regression parameter").equals(0.0000706);
    Input(rh).help("Ambient relative humidity (%)");
    Output(value).help("Grain moisture content (%)");
}

void GrainMoistureContent::reset() {
    update();
}

void GrainMoistureContent::update() {
    value = c1*rh + c2*rh*rh + c3*rh*rh*rh;
}


}
