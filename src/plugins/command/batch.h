/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BATCH_H
#define BATCH_H
#include <QFile>
#include <QTextStream>
#include <base/command.h>

namespace command {

class batch : public base::Command
{
public:
    batch(QString name, base::Box *parent);
private:
    // Data
    QString _relativePath, _fileMask;
    QStringList _filePaths;
    QFile _clipboardFile;
    QTextStream _clipboardStream;
    // Methods
    void doExecute();
    void getArguments();
    void getFilePaths();
    void openFile();
    void runFiles();
    void streamClipboard(QString filePath);
    void fillClipboard();
};

}

#endif
