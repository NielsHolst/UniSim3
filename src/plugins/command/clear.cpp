/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QFont>
#include <QFontDatabase>
#include <QTextCharFormat>
#include <QTextCursor>
#include <QTextEdit>
#include <base/command_help.h>
#include <base/dialog_base.h>
#include <base/publish.h>
#include "clear.h"

using namespace base;

namespace command {

PUBLISH(clear)
HELP(clear, "clear", "clears console window")

clear::clear(QString name, Box *parent)
    : Command(name, parent)
{
}

void clear::doExecute() {
//    QTextCursor cursor = dialog().textEdit()->textCursor();
//    QTextCharFormat format = cursor.charFormat();
//    QFont font = format.font();
//    QString family = font.family();
//    int pt = font.pointSize();

    dialog().textEdit()->clear();
    dialog().restoreFont();

//    format.setFont(QFontDatabase().font(family, QString(), pt));
//    cursor.setCharFormat(format);
//    dialog().textEdit()->setTextCursor(cursor);
}

}
