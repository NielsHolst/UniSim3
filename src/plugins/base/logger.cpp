/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <iostream>
#include <QDateTime>
#include <QDir>
#include "environment.h"
#include "logger.h"

namespace base {

void Logger::open(QString fileNamePath) {
    file.setFileName(fileNamePath);
    file.open(QIODeviceBase::Text | QIODeviceBase::WriteOnly);
    str.setDevice(&file);
}

void Logger::write(QString s) {
    str << qPrintable(s) << "\n";
}


void Logger::close() {
    file.close();
}

}

