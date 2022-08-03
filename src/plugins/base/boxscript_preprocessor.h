/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_BOXSCRIPT_PREPROCESSOR_H
#define BASE_BOXSCRIPT_PREPROCESSOR_H

#include <QMap>
#include <QString>
#include <QVector>

namespace base {

class BoxScriptPreprocessor
{
public:
    struct FileInclude {
        QString sourceFilePath, includeFilePath;
    };
    typedef QVector<FileInclude> FileIncludes;

    BoxScriptPreprocessor();
    QString preprocess(QString filePath);

private:
    // Data
    struct Position {
        int begin, end;
    };
    typedef QVector<Position> Positions;
    QString _origFilePath, _curFilePath;
    // Methods
    QString readFile(QString filePath, FileIncludes earlierIncludes);
    QString expandIncludes(QString sourceFilePath, QString code, FileIncludes earlierIncludes);
    Positions findDirective(QString code, QString directive) const;
    bool isComment(QString s) const;
    QString extractIncludeFilePath(QString includeLine) const;
    void checkCyclicIncludes(FileInclude newFileInclude, FileIncludes earlierIncludes) const;
    QString backtrack(FileIncludes earlierIncludes) const;
    QString replaceMacros(QString code);
    QMap<QString,QString> extractMacros(QString code);
    QString extractUsing(QString code);
    void setUsing(QString pluginName);
};

}

#endif
