/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SAVE_H
#define SAVE_H
#include <base/command.h>

namespace command {

class save : public base::Command
{
public:
    save(QString name, base::Box *parent);
private:
    void doExecute();
    void saveGrammar();
};

}

#endif
