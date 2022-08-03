/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/box_builder.h>
#include <base/publish.h>
#include "shelter_face.h"

using namespace base;

namespace vg {

PUBLISH(ShelterFace)

ShelterFace::ShelterFace(QString name, Box *parent)
    : Box(name, parent)
{
    help("models a shelter face");
}

void ShelterFace::amend() {
    if (!findMaybeOne<Box*>("./area"))
        ThrowException("Shelter face must have an 'area' child box").context(this);
    BoxBuilder builder(this);
    if (!findMaybeOne<Box*>("./cover"))
        builder.box("Cover").name("cover").endbox();
    if (objectName().contains("roof") && !findMaybeOne<Box*>("./vent")) {
        QString name = "vent"; // + objectName().back();
        builder.box("Vent").name(name).endbox();
    }
}

} //namespace

