/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef COMMAND_DOCUMENTATION_H
#define COMMAND_DOCUMENTATION_H
#include <base/documentation.h>
#include <base/publish.h>

namespace command {

class commandDocumentation : public base::Documentation
{
public:
    commandDocumentation(QString name, base::Box *parent);
};

} //namespace

#endif
