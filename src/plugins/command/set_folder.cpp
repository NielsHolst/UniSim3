/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
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
#include "reconfigure.h"
#include "set_folder.h"

using namespace base;

namespace command {

PUBLISH(set_folder)
HELP(set_folder, "set folder", "shows paths for all folder types")
HELP(set_folder_type, "set folder <folder type>", "shows path for this folder type")
HELP(set_folder_type_path, "set folder <folder type> <path>", "changes the path for this folder type")
HELP(set_folder_default, "set folder DEFAULT", "changes all paths to their default value")

set_folder::set_folder(QString name, Box *parent)
    : Command(name, parent)
{
    setClassName("command", "set_folder");
}

void set_folder::doExecute() {
    switch (_args.size()) {
    case 2:
        if (_args.at(1)=="DEFAULT")
            setToDefault();
        else
            showAllFolders();
        break;
    case 3:
        showFolder(_args.at(2));
        break;
    case 4:
        setFolder(_args.at(2), _args.at(3));
        break;
    default:
        ThrowException("Too many arguments").value(_args.join(" "));
    }
}

void set_folder::showAllFolders() {
    QString info =
        "Work folder:\n  " +
        environment().folderInfo(Environment::Work) +
        "\nInput folder:\n  " +
        environment().folderInfo(Environment::Input) +
        "\nOutput folder:\n  " +
        environment().folderInfo(Environment::Output);
//        if (environment().isWindows()) {
//            info +=
//                "\nNotepad++ editor folder:\n  " +
//                environment().folderInfo(Environment::Notepad);
//        }
//        else {
//            info +=
//                "\nAtom editor folder:\n  " +
//                environment().folderInfo(Environment::Atom);
//        }
    dialog().information(info);
}

void set_folder::showFolder(QString folderType) {
    dialog().information( environment().folderInfo(convert<Environment::Folder>(folderType)) );
}

void set_folder::setFolder(QString folderType, QString folderPath) {
    Environment::Folder folder = convert<Environment::Folder>(folderType);
    if (folderPath == "HOME")
        folderPath = reconfigure::destination().absolutePath();
    else if (folderPath == "DEV") {
        QString relativePath = environment().isMac() ? "/../../../.." : "/..";
        folderPath = QApplication::applicationDirPath() + relativePath;
        QDir srcDir = QDir(folderPath + "/src");
        if (!srcDir.exists())
            ThrowException("Could not find the development folder").
                    hint("You must run the unisim.exe that you built yourself (found in the bin folder) to use 'set folder work DEV'");
    }
    environment().dir(folder, folderPath);
    if (folder == Environment::Work)
        showAllFolders();
    else
        showFolder(folderType);
}

void set_folder::setToDefault() {
    QString documents = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).at(0),
            home = QStandardPaths::standardLocations(QStandardPaths::HomeLocation).at(0);
    environment().dir(Environment::Work, documents + "/UniSim");
    environment().dir(Environment::Input, "./input");
    environment().dir(Environment::Output, "./output");
    environment().dir(Environment::Notepad, home + "/AppData/Roaming/Notepad++");
    environment().dir(Environment::Atom, home + "/.atom/packages/language-boxes/grammars");
    set_folder::showAllFolders();
}

}
