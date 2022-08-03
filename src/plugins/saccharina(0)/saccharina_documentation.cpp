/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "saccharina_documentation.h"

using namespace base;

namespace saccharina {

PUBLISH(saccharinaDocumentation)

saccharinaDocumentation::saccharinaDocumentation(QString name, QObject *parent)
    : Documentation(name, parent)
{
    title = "A model of kelp (Saccharina latissima) growth and biomass production";
    authorNames
            << "Teis Boderskov"
            << "Niels Holst";
    authorInstitutes
            << "Aarhus University"
            << "Aarhus University";
    authorEmails
            << "tebo@bios.au.dk"
            << "niels.holst@agro.au.dk";
}

} //namespace

