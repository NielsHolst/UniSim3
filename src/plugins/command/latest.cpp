/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QLocale>
#include <base/command_help.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/publish.h>
#include "latest.h"

using namespace base;

namespace command {

PUBLISH(latest)
HELP(latest, "latest", "shows latest loaded scripts")

latest::latest(QString name, Box *parent)
    : Command(name, parent)
{
}


void latest::doExecute() {
    if (_args.size() > 1)
        ThrowException("'location' takes no arguments");
    QStringList args = environment().latestLoadArgs();
    dialog().information(args.join("\n"));
}


}
