/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QFile>
#include <base/command_help.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/publish.h>
#include "libr.h"

using namespace base;

namespace command {

PUBLISH(libr)
HELP(libr, "libr", "shows the needed R libraries")

libr::libr(QString name, Box *parent)
    : Command(name, parent)
{
}

namespace {
    const QString errorMsg =
        "Write 'libr l' for R code to load libraries\n"
        "Write 'libr i' for R code to install libraries";
}

void libr::doExecute() {
    if (_args.size() != 2)
        ThrowException(errorMsg).value(_args.join(" "));

    QString option = _args.at(1).toLower();
    if (option!="l" && option!="i")
        ThrowException(errorMsg).value(_args.join(" "));

    QString code = readRcode();
    if (option=="l")
        code.replace("install.packages", "library");

    dialog().information(code);
}

QString libr::readRcode() {
    QString fileNamePath = environment().filePath(Environment::Input, "scripts/install_packages.R");
    QFile file(fileNamePath);
    if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
        ThrowException("Could not open file").value(fileNamePath);
    return file.readAll();
}

}
