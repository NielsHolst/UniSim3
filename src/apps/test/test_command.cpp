/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <QDir>
#include <QStringList>
#include <base/command.h>
#include <base/environment.h>
#include <base/mega_factory.h>
#include "test_command.h"

using namespace base;

void TestCommand::testCreate() {
    Command *help = MegaFactory::create<Command>("help", "help", 0);
    QVERIFY(help);
    delete help;
}
void TestCommand::testExecute() {
    Command *help = MegaFactory::create<Command>("help", "help", 0);
//    help->arguments(QStringList() << "cd");
    try {
        help->execute();
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.what()));
    }
    delete help;
}

