/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box_builder.h>
#include <base/publish.h>
#include "indoors.h"

using namespace base;

namespace vg {

PUBLISH(Indoors)

Indoors::Indoors(QString name, Box *parent)
    : Box(name, parent)
{
    help("contains sub-models for indoors climate");
}

void Indoors::amend() {
    BoxBuilder builder(this);
    if (!findMaybeOne<Box*>("./humidity"))
        builder.
        box("vg::IndoorsHumidity").name("humidity").
        endbox();
    if (!findMaybeOne<Box*>("./co2"))
        builder.
        box("vg::IndoorsCo2").name("co2").
        endbox();
    if (!findMaybeOne<Box*>("./windSpeed"))
        builder.
        box("vg::IndoorsWindSpeed").name("windSpeed").
        endbox();
}

} //namespace

