/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SHELTER_FACE_H
#define SHELTER_FACE_H
#include <base/convert.h>

namespace base {
    class Box;
}

namespace vg {

enum class ShelterFace {
    Roof1, Roof2,
    Side1, Side2,
    End1, End2
};

QString toString(ShelterFace face);
ShelterFace toShelterFace(QString name, base::Box *context=nullptr);
QStringList shelterFaceNames();
bool isShelterFaceName(QString name);

}

#endif

