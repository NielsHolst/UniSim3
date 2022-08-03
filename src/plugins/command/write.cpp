/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QDesktopServices>
#include <QFile>
#include <QUrl>
#include <QTextStream>
#include <base/box.h>
#include <base/command_help.h>
#include <base/dialog_base.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/publish.h>
#include "write.h"

using std::unique_ptr;
using namespace base;

namespace command {

PUBLISH(write)
HELP(write, "write", "writes box script")

write::write(QString name, Box *parent)
    : Command(name, parent)
{
}


void write::doExecute() {
    writeFile();
    if (_args.contains("edit"))
        edit();
}

void write::writeFile() {
    Box *root =  Box::root();
    if (!root)
        dialog().error("No box script loaded");
    else {
        QFile file;
        environment().openOutputFile(file, ".box");
        QTextStream text(&file);
        text << root->toText("I");

        _filePath = environment().outputFilePath(".box");
        QString info("Box script written to '%1'");
        dialog().information(info.arg(_filePath));
        environment().incrementFileCounter();
    }
}

void write::edit() {
    QString url = "file:///" + _filePath;
    bool ok = QDesktopServices::openUrl(QUrl(url));
    if (!ok)
        ThrowException("Could not open editor");
}

}
