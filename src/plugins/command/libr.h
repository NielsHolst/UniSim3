/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef LIBR_H
#define LIBR_H
#include <base/command.h>

namespace command {

class libr : public base::Command
{
public:
    libr(QString name, base::Box *parent);
private:
    // Methods
    void doExecute();
    QString readRcode();
};

}

#endif
