/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef INPUT_FILE_PATH_H
#define INPUT_FILE_PATH_H
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <base/exception.h>


inline QDir inputDir() {
    QString path = QCoreApplication::applicationDirPath();
    QDir dir = QDir(path);
    dir.cdUp();
    dir.cd("src/apps/test/input");
    return dir;
}

inline QString inputFilePath(QString fileName) {
    QDir dir = inputDir();
    return dir.absoluteFilePath(fileName);
}

inline void openInputFile(QFile &file, QString fileName) {
    file.setFileName(inputFilePath(fileName));
    bool ok = file.open(QIODevice::Text|QIODevice::ReadOnly);
    if (!ok)
        ThrowException("Cannot open file").value(inputFilePath(fileName));
}


#endif // INPUT_FILE_PATH_H

