/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QTextStream>
#include "environment.h"
#include "exception.h"
#include "save_grammar_notepad.h"

namespace base {

namespace {
    const QString sep = "&#x000D;&#x000A;";
}

SaveGrammarNotepad::SaveGrammarNotepad()
    : SaveGrammarBase()
{
}

QString SaveGrammarNotepad::grammarFilePath() {
    QDir dir = environment().dir(Environment::Notepad);
    return dir.absoluteFilePath("userDefineLang.xml");
}

void SaveGrammarNotepad::writeBeginning() {
    const char fileName[] = ":notepad_grammar_template.xml";
    QFile notepad_template(fileName);
    if ( !notepad_template.open(QIODevice::ReadOnly | QIODevice::Text) )
        ThrowException("Cannot open file").value(fileName);

    QTextStream stream(&_output);
    stream
        << notepad_template.readAll();
}

void SaveGrammarNotepad::writePlugins() {
    _output = _output.replace("#Keywords1", plugInNames().join(sep));
}

void SaveGrammarNotepad::writeClasses() {
    _output = _output.replace("#Keywords2", classNames().join(sep));
}

void SaveGrammarNotepad::writePorts() {
    QStringList decoratedPortNames;
    for (QString portName : portNames()) {
        decoratedPortNames << "." + portName;
    }
    _output = _output.replace("#Keywords3", decoratedPortNames.join(sep));
}

void SaveGrammarNotepad::writeAttributes() {
    _output = _output.replace("#Keywords4", attributeNames().join(sep));
}

void SaveGrammarNotepad::writeTransforms() {
    QStringList names = transformNames();
    names << "#define" << "#include" << "#using";
    _output = _output.replace("#Keywords5", names.join(sep));
}

void SaveGrammarNotepad::writeConstants() {
    _output = _output.replace("#Keywords6", constantNames().join(sep));
}

void SaveGrammarNotepad::writeEnd() {
    _stream <<_output;
}

}
