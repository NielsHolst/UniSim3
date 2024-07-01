/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <QDesktopServices>
#include <QFileInfo>
#include <QUrl>
#include <base/command_help.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/publish.h>
#include "edit.h"

using namespace base;

namespace command {

PUBLISH(edit)
HELP(edit, "edit", "opens box script in editor")

edit::edit(QString name, Box *parent)
    : Command(name, parent)
{
}

void edit::doExecute() {
    Environment &env(environment());
    if (_args.size() > 2)
        ThrowException("The 'edit' command takes 0 or 1 argument");


    bool hasArgument = (_args.size() == 2);
    QString fileName;
    if (hasArgument) {
        fileName = _args.at(1);
    }
    else {
        QString cur = env.currentLoadArg();
        fileName = cur.isEmpty() ? env.latestLoadArg() : cur;
        if (fileName.isEmpty())
            ThrowException("Load a file before editing");
    }

    QString fileNamePath = env.inputFileNamePath(fileName),
            url = "file:///" + fileNamePath;
    if (!QFileInfo::exists(fileNamePath))
        ThrowException("Cannot find file").value(fileNamePath);
    bool ok = QDesktopServices::openUrl(QUrl(url));
    if (!ok)
        ThrowException("Could not open editor");
}

}
