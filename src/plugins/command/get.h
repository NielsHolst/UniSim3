/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef GET_H
#define GET_H
#include <base/command.h>

namespace command {

class get : public base::Command
{
public:
    get(QString name, base::Box *parent);
private:
    void doExecute();
};

}

#endif
