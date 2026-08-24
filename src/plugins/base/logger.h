/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef LOGGER_H
#define LOGGER_H

#include <QFile>
#include <QString>
#include <QTextStream>

#include "base_dllexport.h"

namespace base {

class UNI_API Logger {
public:
    Logger();
    ~Logger();
    void open(QString fileNamePath);
    void write(QString s);
    void close();
private:
    QFile file;
    QTextStream str;
    bool isOpen;
};


}

#endif

