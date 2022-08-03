/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QMainWindow>
#include <base/command_help.h>
#include <base/dialog.h>
#include <base/publish.h>
#include "quit.h"

using namespace base;

namespace command {

PUBLISH(quit)
HELP(quit, "quit", "close console")

quit::quit(QString name, Box *parent)
    : Command(name, parent)
{
    helpText("quit");
}

void quit::doExecute() {
    QMainWindow *mainWindow = dynamic_cast<QMainWindow*>(dialog().parent());
    Q_ASSERT(mainWindow);
    mainWindow->close();
}

}
