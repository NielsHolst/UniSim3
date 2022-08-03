/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "shelter_face_area.h"

using namespace base;

namespace vg {

PUBLISH(ShelterFaceArea)

ShelterFaceArea::ShelterFaceArea(QString name, Box *parent)
    : Box(name, parent)
{
    help("models a shelter face area");
    Input(roofArea).imports("geometry[roofArea]").unit("m2");
    Input(sideWallsArea).imports("geometry[sideWallsArea]").unit("m2");
    Input(endWallsArea).imports("geometry[endWallsArea]").unit("m2");
    Input(gablesArea).imports("geometry[gablesArea]").unit("m2");
    Input(groundArea).imports("geometry[groundArea]").unit("m2");

    Output(value).noClear().help("Surface area").unit("m2");
    Output(relativeArea).noClear().help("Proportion of this surface area out of the total surface area").unit("[0;1]");
    Output(areaPerGround).noClear().help("Surface area relative to the ground area").unit("m2/m2");
}

void ShelterFaceArea::reset() {
    QString name = parent()->objectName();
    if (name.toLower().contains("roof"))
        value = roofArea/2;
    else if (name.toLower().contains("side"))
        value = sideWallsArea/2;
    else if (name.toLower().contains("end"))
        value = endWallsArea/2;
    else {
        QString msg{"Parent's name is '%1' but it must contain 'roof', 'side' or 'end'"};
        ThrowException(msg.arg(name));
    }
    relativeArea = value/(roofArea + sideWallsArea + endWallsArea);
    areaPerGround = value/groundArea;
}

} //namespace

