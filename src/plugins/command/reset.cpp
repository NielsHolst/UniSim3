/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box.h>
#include <base/command_help.h>
#include <base/publish.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include "reset.h"
#include "set_folder.h"

using namespace base;

namespace command {

PUBLISH(reset)
HELP(reset, "reset [boxscript]", "reset simulation")

reset::reset(QString name, Box *parent)
    : Command(name, parent)
{
}

void reset::doExecute() {
    if (_args.size() > 2)
        ThrowException("Command 'reset' takes at most one argument");
    dialog().resetErrorCount();
    doLoad();
    if (dialog().errorCount() == 0)
        doReset();
}

void reset::doLoad() {
    QStringList com;
    com << "load";
    switch (_args.size()) {
    case 1:
        com << environment().latestLoadArg();
        break;
    case 2:
        com << _args[1];
    }
    Command::submit(com);
}

void reset::doReset() {
    Box *sim = Box::root();
    if (!sim) {
        dialog().error("Nothing to reset");
        return;
    }
    sim->initializeFamily();
    sim->resetFamily();
}


}
