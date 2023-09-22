/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box_builder.h>
#include <base/publish.h>
#include "faces.h"

using namespace base;

namespace vg {

PUBLISH(Faces)

Faces::Faces(QString name, Box *parent)
    : Box(name, parent)
{
    help("contains the six greenhouse faces");
}

void Faces::amend() {
    // Ensure that products/screens contain a "none" screen
    Box *noneScreen = findMaybeOne<Box*>("shelter/products/screens/none");
    if (!noneScreen) {
        Box *screens    = findOne<Box*>("shelter/products/screens");
        BoxBuilder builder(screens);
        builder.box("Screen").name("none").
        endbox();
    }
}

} //namespace

