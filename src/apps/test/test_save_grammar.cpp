#include <base/command.h>
#include "test_save_grammar.h"

using namespace base;

void TestSaveGrammar::testExecute() {
    Command::submit(QStringList() << "save" << "grammar", this);
}

