/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/command_help.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/publish.h>
#include "set_autosave.h"

using namespace base;

namespace command {

PUBLISH(set_autosave)
HELP(set_autosave, "set autosave", "shows autosave status")
HELP(set_autosave_path, "set autosave <on/off>", "sets autosave")

set_autosave::set_autosave(QString name, QObject *parent)
    : Command(name, parent)
{
    setClassName("set_autosave");
}


void set_autosave::doExecute() {
    Environment &env(environment());
    DialogBase &dia(dialog());

    switch (_args.size()) {
    case 2:
        break;
    case 3:
        env.state.autosave = value(_args.at(2));
        break;
    default:
        throw Exception("Write 'set autosave <on/off>'");
    }
    dia.information(QString("Autosave is ") + (env.state.autosave ? "on" : "off"));
}

bool set_autosave::value(QString s) {
    if (s.toLower() == "on")
        return true;
    else if (s.toLower() == "off")
        return false;
    throw Exception("Expected 'on' of 'off'", s);
}

}
