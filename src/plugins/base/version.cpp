/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef BASE_VERSION_H
#define BASE_VERSION_H

#include <QFile>
#include <QStringList>

#include "version.h"

namespace  {
    const QString unknown{"(Unknown version)"};
}

namespace base {

UNI_API QString versionRaw() {
    QFile file (":/version.txt");
    if (!file.open(QIODevice::ReadOnly|QIODevice::Text)) {
        return unknown;
    }
    return file.readAll();
}

UNI_API QString version(QString raw) {
    QStringList items = raw.split("\t");
    if (items.size() != 3) {
        return unknown;
    }
    QString code = items.join(".");
    #ifdef QT_DEBUG
        code += " (debug)";
    #endif
    return code;
}

UNI_API QString version() {
    return version(versionRaw());
}

}

#endif

