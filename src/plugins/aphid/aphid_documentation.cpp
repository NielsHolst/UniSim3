/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Stéphanie Saussure, Norwegian Institute of Bioeconomy Research [stephanie.saussure at nibio.no].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "aphid_documentation.h"

using namespace base;

namespace aphid {

PUBLISH(aphidDocumentation)

aphidDocumentation::aphidDocumentation(QString name, Box *parent)
    : Documentation(name, parent)
{
    title = "A model of wheat-aphids-entomopathogenic fungi population dynamics";
    authorNames
            << "StÃ©phanie Saussure"
            << "Niels Holst";
    authorInstitutes
            << "Norwegian Institute of Bioeconomy Research"
            << "Aarhus University";
    authorEmails
            << "stephanie.saussure@gmail.com"
            << "niels.holst@agro.au.dk";
}

} //namespace

