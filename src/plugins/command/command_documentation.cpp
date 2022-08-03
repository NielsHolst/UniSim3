/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "command_documentation.h"

using namespace base;

namespace command {

PUBLISH(commandDocumentation)

commandDocumentation::commandDocumentation(QString name, Box *parent)
    : Documentation(name, parent)
{
    title = "Commands for the prompt";
    authorNames
            << "Niels Holst";
    authorInstitutes
            << "Aarhus University";
    authorEmails
            << "niels.holst@agro.au.dk";
}

} //namespace

