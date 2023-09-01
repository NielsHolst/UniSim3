/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VG_H
#define VG_H
#include <QFile>
#include <QTextStream>
#include <base/command.h>

namespace base {
    class Box;
}

namespace command {

class vg : public base::Command
{
public:
    vg(QString name, base::Box *parent);
private:
    // Data
    QFile _file;
    QTextStream _stream;

    // Methods
    void doExecute();
    void writeScreenCalibration();
    void writeScreenHeader();
    void writeNoScreenReference(base::Box *screen);
    void writeScreenValues(QString name);
};

}

#endif
