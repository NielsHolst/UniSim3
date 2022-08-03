/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SET_AUTOSAVE_H
#define SET_AUTOSAVE_H
#include <base/command.h>

namespace command {

class set_autosave : public base::Command
{
public:
    set_autosave(QString name, QObject *parent);
private:
    void doExecute();
    bool value(QString s);
};

}

#endif
