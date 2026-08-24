/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef BASE_ORGANISATION_H
#define BASE_ORGANISATION_H

#include <QApplication>

#include "version.h"
#include "base_dllexport.h"

namespace base {

struct UNI_API Organisation{
    Organisation() {
        QCoreApplication::setOrganizationName("AarhusUniversity");
        QCoreApplication::setOrganizationDomain("www.ecolmod.org");
        QCoreApplication::setApplicationName("Universal Simulator");
        QCoreApplication::setApplicationVersion(version());
    }
};

UNI_API Organisation organisation;

}

#endif // ORGANISATION_H

