/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/command.h>
#include "test_save_grammar.h"

using namespace base;

void TestSaveGrammar::testExecute() {
    Command::submit(QStringList() << "save" << "grammar", this);
}

