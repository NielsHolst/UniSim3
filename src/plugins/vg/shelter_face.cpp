/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box.h>
#include "shelter_face.h"

using namespace base;

namespace vg {

QString toString(ShelterFace face) {
    static QMap<ShelterFace, QString> map =
    {
        {ShelterFace::Roof1, "roof1"},
        {ShelterFace::Roof2, "roof2"},
        {ShelterFace::Side1, "side1"},
        {ShelterFace::Side2, "side2"},
        {ShelterFace::End1,  "end1"},
        {ShelterFace::End2,  "end2"}
    };
    return map.value(face);
}

ShelterFace toShelterFace(QString name, Box *caller) {
    static QMap<QString, ShelterFace> map =
    {
        {"roof1", ShelterFace::Roof1},
        {"roof2", ShelterFace::Roof2},
        {"side1", ShelterFace::Side1},
        {"side2", ShelterFace::Side2},
        {"end1" , ShelterFace::End1},
        {"end2" , ShelterFace::End2}
    };
    if (!isShelterFaceName(name))
        ThrowException("Invalid name of shelter face").value(name).
                hint("Valid names: " + shelterFaceNames().join(", ")).
                context(caller);
    return map.value(name);

}

QStringList shelterFaceNames() {
    static QStringList names =
        {"roof1", "roof2", "side1", "side2", "end1", "end2"};
    return names;
}

bool isShelterFaceName(QString name) {
    return shelterFaceNames().contains(name);
}


}

