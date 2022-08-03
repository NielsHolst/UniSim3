/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef HT_H
#define HT_H
#include <base/command.h>

namespace base {
    class Box;
}

namespace command {

class ht : public base::Command
{
public:
    ht(QString name, base::Box *parent);
private:
    void doExecute();
};

}

#endif
