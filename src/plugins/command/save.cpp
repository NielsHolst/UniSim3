/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/command_help.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/publish.h>
#include <base/save_grammar_atom.h>
#include <base/save_grammar_notepad.h>
#include "save.h"

using namespace base;

namespace command {

PUBLISH(save)
//HELP(save_grammar, "save grammar", "save grammar for Atom and Notepad++ editors")

save::save(QString name, Box *parent)
    : Command(name, parent)
{
}

void save::doExecute() {
    bool ok(true);
    if (_args.size() == 2) {
        QString arg = _args.at(1);
        if (arg == "grammar")
            saveGrammar();
        else
            ok = false;
    }
    else
        ok = false;
    if (!ok)
        dialog().error("Write: 'save grammar'");
}

void save::saveGrammar() {
    bool atomOk = environment().dir(Environment::Atom).exists(),
         notepadOk = environment().dir(Environment::Notepad).exists();
    if (atomOk)
        SaveGrammarAtom().write();
    if (notepadOk)
        SaveGrammarNotepad().write();
}

}
