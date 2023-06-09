/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "face.h"

using namespace base;

namespace vg {

PUBLISH(Face)

Face::Face(QString name, Box *parent)
    : Box(name, parent)
{
    help("models the greenhouse faces");
    Input(cover).equals("glass").help("A material listed in shelter/covers");
    Input(screens).help("Zero or more screen materials listed in shelter/screens");
    Input(area).unit("m2").help("Area of face");
    Input(weight).equals(1.).unit("[0;1]").help("Weight given to face in radiation budget");
    Output(screenMaterials).help("Vector of screen material names");
}

void Face::initialize() {
    QStringList list = screens.split("+");
    screenMaterials = QVector<QString>(list.cbegin(), list.cend());
}

} //namespace

