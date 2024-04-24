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
    QStringList _userFilePaths, _boxFilePaths, _batchRFilePaths;
    QFile _file;
    QTextStream _fileStream;
    bool _isFirstFigure;
    // Methods
    void doExecute();
    void getArguments();
    QStringList getFilePaths(QString fileMask);
    QStringList getFilePaths(QDir dir, QString fileMask);
    void openFile();
    void closeFile();
    void runBoxFiles(QStringList boxFilePaths);
    void sourceRFiles();
    QString readFile(QString filePath) const;
    bool fileContainsGgplot(QString filePath) const;
    bool pagesExpected(QString filePath);
    void writeClipboard();
    void writeFigure(QString filePath, QString fileType, bool pagesExpected);
    void fillClipboard();
};

}

#endif
