/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef BASE_VERSION_H
#define BASE_VERSION_H

#include <QString>

#include "base_dllexport.h"

namespace base {
    UNI_API QString version();
    UNI_API QString versionRaw();
    UNI_API QString version(QString raw);
}

#endif

