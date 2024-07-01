/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/box.h>
#include <base/command_help.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/publish.h>
#include "general.h"
#include "rows.h"

using namespace base;

namespace command {

PUBLISH(rows)
HELP(rows, "rows", "show rows of output data frame")

rows::rows(QString name, Box *parent)
    : Command(name, parent)
{
}

void rows::doExecute() {
    int from, to;
    bool error = true;
    if (_args.size() == 3) {
        bool ok1, ok2;
        from = _args.at(1).toInt(&ok1),
        to   = _args.at(2).toInt(&ok2);
        error = !(ok1 && ok2);
    }
    if (error)
        dialog().error("Write: rows <from row number> <to row number>");

    QFile file;
    QString filePath = environment().latestOutputFilePath("txt");
    file.setFileName(filePath);
    if ( !file.open(QIODevice::ReadOnly | QIODevice::Text) )
        ThrowException("Could not open file").value(filePath);

    QString header = readHeader(file);
    skipLines(file, from-1);
    QStringList lines = readLines(file, to-from+1);
    lines.prepend(header);

    QVector<int> widths = getWidths(lines);
    QStringList output = rightJustify(lines, widths);

    dialog().information("Showing " + filePath);
    for (auto line : output)
        dialog().information(line);
}

}
