/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** William Meikle, USDA [william.meikle at usda.gov].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "cmbp_documentation.h"

using namespace base;

namespace cmbp {

PUBLISH(cmbpDocumentation)

cmbpDocumentation::cmbpDocumentation(QString name, Box *parent)
    : Documentation(name, parent)
{
    title = "Models of grain store pest population dynamics";
    authorNames
            << "Niels Holst"
            << "William Meikle";
    authorInstitutes
            << "Aarhus University"
            << "USDA-ARS, Tucson, AZ";
    authorEmails
            << "niels.holst@agro.au.dk"
            << "william.meikle@usda.gov";
}

} //namespace

