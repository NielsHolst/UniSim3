/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_VERSION_H
#define BASE_VERSION_H
#include <QFile>
#include <QStringList>
#include "exception.h"
#include "version.h"

namespace  {
    const QString unknown{"(Unknown version)"};
}

namespace base {

QString versionRaw() {
    QFile file (":/version.txt");
    if (!file.open(QIODevice::ReadOnly|QIODevice::Text)) {
        return unknown;
    }
    return file.readAll();
}

QString version(QString raw) {
    QStringList items = raw.split("\t");
    if (items.size() != 4) {
        return unknown;
    }
    QString config = items.at(3);
    items.removeAt(3);
    QString code = items.join(".");
    if (config != "release")
        code += " (developer)";
    return code;
}

QString version() {
    return version(versionRaw());
}

}

#endif

