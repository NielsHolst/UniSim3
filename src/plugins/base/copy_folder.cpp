/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QDirIterator>
#include <QFile>
#include <QFileInfo>
#include "copy_folder.h"
#include "dialog.h"
#include "exception.h"

namespace base {

namespace {

    QString numbered(QString s, int i) {
        return s + "_" + QString::number(i);
    }

    void makeWritable(QString fileNamePath) {
        QFile::setPermissions(fileNamePath,
                              QFileDevice::ReadOwner|QFileDevice::WriteOwner|
                              QFileDevice::ReadUser|QFileDevice::WriteUser|
                              QFileDevice::ReadGroup|QFileDevice::WriteGroup|
                              QFileDevice::ReadOther|QFileDevice::WriteOther
                              );
    }

}

void copyFolder(QDir source, QDir destination, QString sourceText) {
    if (destination.exists())
        renameFolder(destination);
    QString msg = "Copying %1 to %2\n";
    dialog().information(msg.arg(sourceText).arg(destination.absolutePath()));
    copyFolderHard(source, destination);
}

void renameFolder(QDir dir) {
    QString oldPath = dir.absolutePath();
    int i = 1;
    while (QDir(numbered(oldPath, i)).exists())
        ++i;
    QString newPath = numbered(oldPath, i);

    QString info = oldPath + " -> " + newPath;
    dialog().information("Renaming folder: " + info);

    bool ok = QDir().rename(oldPath, newPath);
    if (!ok)
        ThrowException("Cannot rename folder").value(info)
                      .hint("Close programs using the folder then try again.\nMost often, just close Windows File Explorer");
}

void copyFolderHard(QDir source, QDir destination) {
    // Create destination folder if needed; folders along the path are also created if needed
    if (!destination.exists()) {
        bool ok = QDir().mkpath(destination.absolutePath());
        if (!ok)
            ThrowException("Cannot create destination folder").value(destination.absolutePath())
                    .hint("Close programs using the destination folder then try again");
    }

    // Copy folder
    QDirIterator it(source);
    while (it.hasNext()) {
        it.next();
        QFileInfo info = it.fileInfo();
        QString entryName = it.fileName(),
                sourcePath = it.filePath(),
                destinationPath = destination.absolutePath() + "/" + entryName;
        if (entryName == "." || entryName == "..")
            continue;
        if (info.isDir())
            copyFolderHard(QDir(sourcePath), QDir(destinationPath));
        else
            copyFile(sourcePath, destinationPath);
    }
}

void copyFile(QString sourcePath, QString destinationPath) {
    QFile source(sourcePath),
          destination(destinationPath);
    if (!source.exists())
        ThrowException("Source file does not exist").value(sourcePath);
    destination.remove();
    destination.close();
    bool fileCopied = source.copy(destinationPath);
    if (!fileCopied)
        ThrowException("Cannot copy file").value(sourcePath+" -> "+destinationPath);
    // Avoid read-only permission
    makeWritable(destinationPath);
}

} //namespace
