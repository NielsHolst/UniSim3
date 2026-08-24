/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef BASE_COPY_FOLDER_H
#define BASE_COPY_FOLDER_H
#include <QDir>
#include <QString>

#include "base_dllexport.h"

namespace base {

UNI_API void copyFolder(QDir source, QDir destination, QString sourceText);
UNI_API void copyFolderHard(QDir source, QDir destination);
UNI_API void copyFile(QString sourcePath, QString destinationPath);
UNI_API void renameFolder(QDir dir);

} //namespace

#endif
