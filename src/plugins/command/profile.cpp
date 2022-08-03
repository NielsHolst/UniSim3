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
#include "profile.h"

using namespace base;

namespace command {

PUBLISH(profile)
HELP(profile, "profile", "writes execution profile to output file")

profile::profile(QString name, Box *parent)
    : Command(name, parent)
{
}

void profile::doExecute() {
    Box *root = Box::root();
    if (_args.size() > 2) {
        throw Exception("Command 'profile' takes no arguments");
    }
    else if (root) {
        QFile file;
        environment().openOutputFile(file, ".txt");
        file.write(qPrintable(root->profileReport()));
        dialog().information("Profile information written to '" + environment().latestOutputFilePath("txt") + "'");
        file.close();
        environment().incrementFileCounter();
    }
    else
        throw Exception("No box loaded");
}

}
