/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>
#include <base/command_help.h>
#include <base/publish.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include "get_folders.h"

using namespace base;

namespace command {

PUBLISH(get_folders)
HELP(get_folders, "get folders", "shows paths to input/output folders")

get_folders::get_folders(QString name, Box *parent)
    : Command(name, parent)
{
    setClassName("command", "get_folders");
}

void get_folders::doExecute() {
    showAllFolders();
}

void get_folders::showAllFolders() {
    QString info =
        "Work folder:\n  " +
        environment().folderInfo(Environment::Work) +
        "\nInput folder:\n  " +
        environment().folderInfo(Environment::Input) +
        "\nOutput folder:\n  " +
        environment().folderInfo(Environment::Output);
    dialog().information(info);
}


}
