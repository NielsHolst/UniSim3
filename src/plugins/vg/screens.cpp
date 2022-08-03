/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box_builder.h>
#include <base/phys_math.h>
#include <base/publish.h>
#include <base/vector_op.h>
#include "screens.h"

using namespace base;

namespace vg {

PUBLISH(Screens)

Screens::Screens(QString name, Box *parent)
    : Box(name, parent)
{
    help("holds Screen children");
}

void Screens::amend() {
    BoxBuilder builder(this);
    if (!findMaybeOne<Box*>("./airTransmissivity"))
        builder.box("vg::ScreensAirTransmissivity").name("airTransmissivity").
        endbox();
}


} //namespace

