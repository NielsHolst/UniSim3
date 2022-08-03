/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "environment.h"
#include "exception.h"
#include "save_grammar_atom.h"

namespace base {

SaveGrammarAtom::SaveGrammarAtom()
    : SaveGrammarBase()
{
}

QString SaveGrammarAtom::grammarFilePath() {
    QDir dir = environment().dir(Environment::Atom);
    return dir.absoluteFilePath("boxes.cson");
}

void SaveGrammarAtom::writeBeginning() {
    const char fileName[] = ":boxes_head.cson";
    QFile beginning(fileName);
    if ( !beginning.open(QIODevice::ReadOnly | QIODevice::Text) )
        ThrowException("Cannot open file").value(fileName);
    _stream
        << beginning.readAll()
        << "\n# Generated by Universal Simulator from here\n";
}

void SaveGrammarAtom::writePlugins() {
    writePattern(plugInNames(), "support.class");
}

void SaveGrammarAtom::writeClasses() {
    writePattern(classNames(), "support.class");
}

void SaveGrammarAtom::writePorts() {
    writePattern(portNames(), "support.variable");
}

void SaveGrammarAtom::writeAttributes() {
    writePattern(attributeNames(), "keyword.control.variable");
}

void SaveGrammarAtom::writeTransforms() {
//    writePattern(transformNames(), "support.function"); // not used
}

void SaveGrammarAtom::writeConstants() {
    writePattern(constantNames(), "constant.language");
}

void SaveGrammarAtom::writeEnd() {
    _stream
        << "]\n"
        << "# Generated by Universal Simulator until here\n";
}

void SaveGrammarAtom::writePattern(QStringList match, QString name) {
    _stream
        << "  {\n"
        << "    'match': '\\\\b(" << match.join("|") << ")\\\\b'\n"
        << "    'name': '" << name << "'\n"
        << "  }\n";
}

}
