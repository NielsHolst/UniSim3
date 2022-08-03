/* Copyright 2020-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "xpest_documentation.h"

using namespace base;

namespace xpest {

PUBLISH(xpestDocumentation)

xpestDocumentation::xpestDocumentation(QString name, QObject *parent)
    : Documentation(name, parent)
{
    title = "XPEST model";
    authorNames
            << "Niels Holst";
    authorInstitutes
            << "Aarhus University";
    authorEmails
            << "niels.holst@agro.au.dk";
}

} //namespace

