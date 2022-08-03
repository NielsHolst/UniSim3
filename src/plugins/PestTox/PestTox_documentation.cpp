/* Copyright 2016-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz.].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "PestTox_documentation.h"

using namespace base;

namespace PestTox {

PUBLISH(PestToxDocumentation)

PestToxDocumentation::PestToxDocumentation(QString name, Box *parent)
    : Documentation(name, parent)
{
    title = "A pesticide eco-toxicological model";
    authorNames
            << "Mwema Felix"
            << "Niels Holst";
    authorInstitutes
            << "Tropical Pesticides Research Institute, Arusha"
            << "Aarhus University";
    authorEmails
            << "mwema.mwema@tpri.go.tz"
            << "niels.holst@agro.au.dk";
}

} //namespace

