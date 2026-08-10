/* Copyright 2025-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Liyan Xie, Aarhus University [liyan.xie at agro.au.dk] and 
** Anne Dahl Svendsen, Aarhus University [anne.svendsen at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/

#include <base/publish.h>
#include "collembola_documentation.h"

using namespace base;

namespace collembola {

PUBLISH(collembolaDocumentation)

collembolaDocumentation::collembolaDocumentation(QString name, Box *parent)
    : Documentation(name, parent)
{
    title = "Collembola model";
    authorNames
            << "Niels Holst"
            << "Liyan Xie"
            << "Anne Dahl Svendsen";
    authorInstitutes
            << "Aarhus University"
            << "Aarhus University"
            << "Aarhus University";
    authorEmails
            << "niels.holst@agro.au.dk"
            << "liyan.xie@agro.au.dk"
            << "anne.svendsen@agro.au.dk";
}

} //namespace



