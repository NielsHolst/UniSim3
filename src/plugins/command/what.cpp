/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <stdio.h>
#include <QFileInfo>
#include <base/command_help.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/publish.h>
#include "what.h"

using namespace base;

namespace command {

PUBLISH(what)
HELP(what, "what", "shows name of current script")

what::what(QString name, Box *parent)
    : Command(name, parent)
{
}

void what::doExecute() {
    if (_args.size() > 1)
        ThrowException("'what' takes no arguments");

    QString fileName =  environment().latestLoadArg();
    QFileInfo fi(fileName);
    QString msg = "Current script is '" + fileName + "'";
    if (!fi.isAbsolute())
        msg += "\nInput folder:\n" + environment().folderInfo(Environment::Input);

    dialog().information(msg);
}


}
