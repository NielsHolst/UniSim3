/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_SAVE_GRAMMAR_NOTEPAD_H
#define BASE_SAVE_GRAMMAR_NOTEPAD_H
#include <QString>
#include "save_grammar_base.h"

namespace base {

class SaveGrammarNotepad : public SaveGrammarBase
{
public:
    SaveGrammarNotepad();
private:
    // data
    QString _output;
    // methods
    QString grammarFilePath();
    void writeBeginning();
    void writePlugins();
    void writeClasses();
    void writePorts();
    void writeAttributes();
    void writeTransforms();
    void writeConstants();
    void writeEnd();
};

}

#endif
