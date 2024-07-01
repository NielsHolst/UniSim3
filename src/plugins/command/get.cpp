/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/command_help.h>
#include <base/publish.h>
#include <base/dialog.h>
#include <base/exception.h>
#include "get.h"
#include "get_folders.h"

using namespace base;

namespace command {

PUBLISH(get)
HELP(get_any, "get", "shows what you can get")

get::get(QString name, Box *parent)
    : Command(name, parent)
{
    setClassName("command", "get");
}

void get::doExecute() {
    std::unique_ptr<Command> command;
    int n = _args.size();
    if (n == 1) {
        dialog().error("Write: 'get folders'");
        return;
    }
    else {
        Q_ASSERT(n > 1);
        QString a1 = _args.at(1);
        if (a1 == "folders")
            command = std::unique_ptr<Command>( new get_folders("get_folders", nullptr) );
        else
            ThrowException("Unknown command").value(_args.join(" "));
    }
    if (command) {
        command->arguments(_args);
        command->execute();
    }
}


}
