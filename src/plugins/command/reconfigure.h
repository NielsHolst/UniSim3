/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef RECONFIGURE_H
#define RECONFIGURE_H
#include <QDir>
#include <base/command.h>

namespace command {

class reconfigure : public base::Command
{
public:
    reconfigure(QString name, base::Box *parent);
    static QDir source();
    static QDir destination();
private:
    void doExecute();
};

}

#endif
