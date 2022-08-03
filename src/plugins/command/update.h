/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef UPDATE_H
#define UPDATE_H
#include <base/command.h>

namespace command {

class update : public base::Command
{
public:
    update(QString name, QObject *parent);
private:
    void doExecute();
};

}

#endif
