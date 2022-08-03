/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_SAVE_GRAMMAR_BASE_H
#define BASE_SAVE_GRAMMAR_BASE_H
#include <QFile>
#include <QSet>
#include <QTextStream>

namespace base {

class Box;

class SaveGrammarBase
{
public:
    SaveGrammarBase();
    void write();
protected:
    // data
    QFile _file;
    QTextStream _stream;
    // methods
    QStringList plugInNames();
    QStringList classNames();
    QStringList portNames();
    QStringList attributeNames();
    QStringList transformNames();
    QStringList constantNames();
private:
    // methods
    void openFileStream();
    void closeFileStream();
    QSet<QString> collectPortNames(Box *root);
    virtual QString grammarFilePath() = 0;
    virtual void writeBeginning() = 0;
    virtual void writePlugins() = 0;
    virtual void writeClasses() = 0;
    virtual void writePorts() = 0;
    virtual void writeAttributes() = 0;
    virtual void writeTransforms() = 0;
    virtual void writeConstants() = 0;
    virtual void writeEnd() = 0;
};

}

#endif
