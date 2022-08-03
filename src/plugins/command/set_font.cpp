/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/command_help.h>
#include <base/dialog.h>
#include <base/exception.h>
#include <base/publish.h>
#include <QFont>
#include <QFontDatabase>
#include <QTextCharFormat>
#include <QTextCursor>
#include "set_font.h"

using namespace base;

namespace command {

PUBLISH(set_font)
HELP(set_font, "set font", "shows console font name and size")
HELP(set_font_name, "set font <font-name>", "sets console font name")
HELP(set_font_size, "set font <integer>", "sets console font size")
HELP(set_font_name_size, "set font <font-name> <integer>", "sets console font name and size")

set_font::set_font(QString name, Box *parent)
    : set(name, parent)
{
    setClassName("command", "set_font");
    _previousFamily = currentFamily();
    _previousPointSize = currentPointSize();
}

QString set_font::currentFamily() {
    return dialog().font().family();
}

int set_font::currentPointSize() {
    return dialog().font().pointSize();
}

void set_font::doExecute() {
    bool ok;
    int pt;
    QStringList &a(_args);
    QString hint{"\nIf font name contains spaces, enclose it in apostrophes"};

    switch(a.size()) {
    case 2:
        showFontUnchanged();
        break;
    case 3:
        pt = a[2].toInt(&ok);
        if (ok)
            setFont(pt);
        else
            setFont(a[2]);
        break;
    case 4:
        pt = a[3].toInt(&ok);
        if (ok)
            setFont(a[2], pt);
        else
            ThrowException("Last argument must be a number").value(_args.join(" ")).hint(hint);
        break;
    default:
        QString s{"Too many arguments (%1): %2"};
        ThrowException(s.arg(a.size()).arg(a.join(" "))).hint(hint);
    }
}

void set_font::showFontUnchanged() {
    QString s0{"Font set to %1 %2pt"};
    QString s = s0
                .arg(currentFamily())
                .arg(currentPointSize());
    dialog().information(s);
}

void set_font::showFont() {
    QString s0{"Font set to %1 %2pt (was %3 %4pt)"};
    QString s = s0
                .arg(currentFamily())
                .arg(currentPointSize())
                .arg(_previousFamily)
                .arg(_previousPointSize);
    dialog().information(s);
}

void set_font::setFont(int pt) {
    dialog().setFont(currentFamily(), pt);
    showFont();
}

void set_font::setFont(QString family) {
    // Show all fonts
    if (family == "ALL") {
        QStringList fam = QFontDatabase().families();
        dialog().information(fam.join("\n"));
    }
    // Change font family
    else {
        if (family == "courier") family = "Courier"; // Lowercase "courier" crashes in Windows
        dialog().setFont(family, currentPointSize());
        showFont();
    }
}

void set_font::setFont(QString family, int pt) {
    dialog().setFont(family, pt);
    showFont();
}



}
