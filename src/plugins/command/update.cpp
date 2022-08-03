/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box.h>
#include <base/command_help.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/publish.h>
#include "update.h"

using namespace base;

namespace command {

PUBLISH(update)
HELP(update, "update", "updates the root box")

update::update(QString name, QObject *parent)
    : Command(name, parent)
{
}

void update::doExecute() {
    Box *root = environment().root();
    if (_args.size() > 1) {
        ThrowException("Command 'update' takes no arguments");
    }
    else if (root) {
        try {
            root->updateFamily();
        }
        catch (Exception &ex) {
            ThrowException("Update: " + root->objectName() + " interrupted\n" + ex.what());
        }
        dialog().information("Update: " + root->objectName() + " finished");
    }
    else
        ThrowException("No box loaded");
}

}
