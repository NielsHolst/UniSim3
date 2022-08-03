/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box.h>
#include <base/command_help.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/publish.h>
#include "debug.h"

#include <base/dialog.h>

using namespace base;

namespace command {

PUBLISH(debug)
HELP(debug, "debug", "runs simulation in debug mode")

debug::debug(QString name, Box *parent)
    : Command(name, parent)
{
}

void debug::doExecute() {
    if (_args.size() > 2)
        ThrowException("Command 'debug' takes at most one argument");
    QStringList com;
    com << "run";
    if (_args.size() == 2)
        com << _args[1];
    Box::debug(true);
    Command::submit(com);
    Box::debug(false);
}

}
