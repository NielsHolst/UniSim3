/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef FIND_H
#define FIND_H
#include <QVector>
#include <base/command.h>

namespace command {

class find : public base::Command
{
public:
    find(QString name, base::Box *parent);
private:
    // methods
    void doExecute();
};

}

#endif
