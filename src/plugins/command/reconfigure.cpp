/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QStandardPaths>
#include <base/command_help.h>
#include <base/copy_folder.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/publish.h>
#include "reconfigure.h"

using namespace base;

namespace command {

PUBLISH(reconfigure)
HELP(reconfigure, "reconfigure", "reconfigures all standard folders and files")

reconfigure::reconfigure(QString name, Box *parent)
    : Command(name, parent)
{
}

void reconfigure::doExecute() {
    copyFolder(source(), destination(), "default input scripts");
}

QDir reconfigure::source() {
    return QDir(":/data");
}

QDir reconfigure::destination() {
    return QDir(environment().homePath());
}

}
