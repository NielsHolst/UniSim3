/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef LOAD_H
#define LOAD_H
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <base/command.h>

namespace command {

class load : public base::Command
{
public:
    load(QString name, base::Box *parent);
private:
    // Methods
    void doExecute();
    void readFile(QString fileName);
};

}

#endif
