/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef PREP_H
#define PREP_H
#include <base/command.h>

namespace command {

class prep : public base::Command
{
public:
    prep(QString name, base::Box *parent);
private:
    // Methods
    void doExecute();
    QString inputFilePath();
    void preprocess();
};

}

#endif
