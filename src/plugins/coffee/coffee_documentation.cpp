/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "coffee_documentation.h"

using namespace base;

namespace coffee {

PUBLISH(coffeeDocumentation)

coffeeDocumentation::coffeeDocumentation(QString name, QObject *parent)
    : Documentation(name, parent)
{
    title = "A model of coffee production based on CAF2018";
    authorNames
            << "Niels Holst"
            << "Athina Koutouleas"
            << "Thuan Sarzynski";
    authorInstitutes
            << "Aarhus University"
            << "University of Copenhagen"
            << "SupAgro Montpellier";
    authorEmails
            << "niels.holst@agro.au.dk"
            << "atk@ign.ku.dk"
            << "thuan.sarzynski@cirad.fr";
}

} //namespace

