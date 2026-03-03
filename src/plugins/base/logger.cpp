/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <QDateTime>
#include <QDir>
#include "dialog_base.h"
#include "logger.h"

namespace base {

Logger::Logger()
    : isOpen(false)
{
}

Logger::~Logger() {
    close();
    isOpen = false;
}

void Logger::open(QString fileNamePath) {
    file.setFileName(fileNamePath);
    bool ok = file.open(QIODevice::Text | QIODevice::WriteOnly);
    if (ok)
        str.setDevice(&file);
    isOpen = ok;
}

void Logger::write(QString s) {
    if (isOpen)
        str << qPrintable(s) << "\n";
}


void Logger::close() {
    if (isOpen) {
        dialog().information("Warning: Test output written to " + file.fileName());
        file.close();
    }
    isOpen = false;
}

}

