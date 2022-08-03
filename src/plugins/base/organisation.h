/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_ORGANISATION_H
#define BASE_ORGANISATION_H

#include <QApplication>
#include "version.h"

namespace base {

struct Organisation{
    Organisation() {
        QCoreApplication::setOrganizationName("AarhusUniversity");
        QCoreApplication::setOrganizationDomain("www.ecolmod.org");
        QCoreApplication::setApplicationName("Universal Simulator");
        QCoreApplication::setApplicationVersion(version());
    }
};

Organisation organisation;

}

#endif // ORGANISATION_H

