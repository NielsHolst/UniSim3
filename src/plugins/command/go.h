/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef GO_H
#define GO_H
#include <QVector>
#include <base/command.h>

namespace command {

class go : public base::Command
{
public:
    go(QString name, base::Box *parent);
private:
    // data
    // methods
    void doExecute();
};

}

#endif
