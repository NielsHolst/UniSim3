/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef RESET_H
#define RESET_H
#include <base/command.h>

namespace command {

class reset : public base::Command
{
public:
    reset(QString name, base::Box *parent);
private:
    void doExecute();
    void doLoad();
    void doReset();
};

}

#endif
