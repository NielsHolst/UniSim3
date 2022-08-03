/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_COPY_FOLDER_H
#define BASE_COPY_FOLDER_H
#include <QDir>
#include <QString>


namespace base {

void copyFolder(QDir source, QDir destination, QString sourceText);
void copyFolderHard(QDir source, QDir destination);
void copyFile(QString sourcePath, QString destinationPath);
void renameFolder(QDir dir);

} //namespace

#endif
