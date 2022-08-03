/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box.h>
#include <base/command_help.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/publish.h>
#include "general.h"
#include "tail.h"

using namespace base;

namespace command {

PUBLISH(tail)
HELP(tail, "tail", "show end of output data frame")

tail::tail(QString name, Box *parent)
    : Command(name, parent)
{
}

void tail::doExecute() {
    int n = 6;
    if (_args.size() == 2) {
        bool ok;
        n = _args.at(1).toInt(&ok);
        if (!ok)
            ThrowException("Number of lines must be specified as an integer");
    }
    else if (_args.size() > 2) {
        dialog().error("Write 'tail' or 'tail <integer>");
    }

    QFile file;
    QString filePath = environment().latestOutputFilePath("txt");
    file.setFileName(filePath);
    if ( !file.open(QIODevice::ReadOnly | QIODevice::Text) )
        ThrowException("Could not open file").value(filePath);

    QString header = readHeader(file);
    QStringList lines = readTailLines(file, n);
    lines.prepend(header);

    QVector<int> widths = getWidths(lines);
    QStringList output = rightJustify(lines, widths);
    dialog().information("Showing " + filePath);
    for (auto line : output)
        dialog().information(line);
}

}
