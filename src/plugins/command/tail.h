/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef TAIL_H
#define TAIL_H
#include <base/command.h>

namespace base {
    class Box;
}

namespace command {

class tail : public base::Command
{
public:
    tail(QString name, base::Box *parent);
private:
    void doExecute();
};

}

#endif
