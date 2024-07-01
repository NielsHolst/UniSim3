/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef DIAGNOSE_H
#define DIAGNOSE_H
#include <base/command.h>

namespace command {

class diagnose : public base::Command
{
public:
    diagnose(QString name, base::Box *parent);
private:
    // Methods
    void doExecute();
};

}

#endif
