/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/publish.h>
#include "actuators.h"

using namespace base;

namespace vg {

PUBLISH(Actuators)

Actuators::Actuators(QString name, Box *parent)
    : Box(name, parent)
{
    help("contains sub-models for actuators");
}

void Actuators::amend() {
    BoxBuilder builder(this);
    if (!findMaybeOne<Box*>("./screens")) {
        builder.
            box().name("screens");
            addLayer(builder, 1);
            addLayer(builder, 2);
            addLayer(builder, 3);
        builder.
            endbox();
    }
}

void Actuators::addLayer(base::BoxBuilder &builder, int i) {
    QString layer = "layer" + QString::number(i);
    builder.
        box("Accumulator").name(layer).
            port("change").imports("./pid[controlVariable]").
            port("minValue").equals(0.).
            port("maxValue").imports("gh/setpoints[maxScreen]").
            box("PidController").name("pid").
                port("desiredValue").imports("controllers/screens/" + layer + "[value]").
                port("sensedValue").imports("..[value]").
                port("Kprop").equals(0.05).
            endbox().
        endbox();
}

} //namespace

