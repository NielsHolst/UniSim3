/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef DEBUG_H
#define DEBUG_H
#include <base/command.h>

namespace command {

class debug : public base::Command
{
public:
    debug(QString name, base::Box *parent);
private:
    // Methods
    void doExecute();
};

}

#endif
