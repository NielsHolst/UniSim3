/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/command_help.h>
#include <base/publish.h>
#include <base/dialog.h>
#include <base/exception.h>
#include "set.h"
#include "set_font.h"
#include "set_folder.h"

using namespace base;

namespace command {

PUBLISH(set)
HELP(set_any, "set", "shows what can be set")

set::set(QString name, Box *parent)
    : Command(name, parent)
{
    setClassName("command", "set");
}

void set::doExecute() {
    std::unique_ptr<Command> command;
    int n = _args.size();
    if (n == 1) {
        dialog().error("Write: 'set folder' or 'set font'");
        return;
    }
    else {
        Q_ASSERT(n > 1);
        QString a1 = _args.at(1);
        if (a1 == "font")
            command = std::unique_ptr<Command>( new set_font("set_font", nullptr) );
        else if (a1 == "folder")
            command = std::unique_ptr<Command>( new set_folder("set_font", nullptr) );
        else
            ThrowException("Unknown command").value(_args.join(" "));
    }
    if (command) {
        command->arguments(_args);
        command->execute();
    }
}


}
