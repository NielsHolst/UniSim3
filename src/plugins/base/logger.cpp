/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <iostream>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include "environment.h"
#include "logger.h"

namespace base {

void log(QString s) {
//    QString fileNamePath = environment().filePath(Environment::Folder::Output, "/unisim_log.txt");
    QString fileNamePath = "C:/Users/au152367/Documents/QDev/UniSim3/output/unisim_log.txt";
    std::cout << qPrintable("Logging to: " + fileNamePath) << "\n";

    QString timeStamp = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss.zzz");

    QFile file;
    file.setFileName(fileNamePath);
    file.open(QIODevice::Text|QIODevice::Append);
    QTextStream str(&file);
    str << qPrintable(timeStamp + " " + s) << "\n";
    file.close();
}

}

