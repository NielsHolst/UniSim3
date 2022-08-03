/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_VERSION_H
#define BASE_VERSION_H
#include <QString>

namespace base {
    QString version();
    QString versionRaw();
    QString version(QString raw);
}

#endif

