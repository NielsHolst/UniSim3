/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "saccharina_documentation.h"

using namespace base;

namespace saccharina {

PUBLISH(saccharinaDocumentation)

saccharinaDocumentation::saccharinaDocumentation(QString name, QObject *parent)
    : Documentation(name, parent)
{
    title = "Student playing ground";
    authorNames
            << "Niels Holst";
    authorInstitutes
            << "Aarhus University";
    authorEmails
            << "niels.holst@agro.au.dk";
}

} //namespace

