/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QMainWindow>
#include "dialog.h"
#include "exception.h"
#include "dialog_widget.h"
#include "history.h"
#include "win_taskbar_progress.h"

namespace base {

Dialog::Dialog(QObject *parent)
    : DialogBase(parent)
{
    setObjectName("dialog");
    QMainWindow *wParent = dynamic_cast<QMainWindow*>(parent);
    if (!wParent)
        ThrowException("Parent must be a QWidget");
    _widget = new DialogWidget(wParent);
}

void Dialog::init() {
    _widget->init();
}

QTextEdit* Dialog::textEdit() {
    return _widget;
}

const History* Dialog::history() const {
    return _widget->history();
}

void Dialog::setFont(QString family, int pt) {
    _widget->setFont(family, pt);
}

QFont Dialog::font() {
    return _widget->font();
}

void Dialog::writePrompt() {
    _widget->writePrompt();
}

QProgressBar* Dialog::progressBar() {
    return _widget->progressBar();
}

QWinTaskbarProgress* Dialog::winProgressTaskbar() {
    return _widget->winProgressTaskbar();
}

void Dialog::finishProgress() {
    _widget->finished();
}

void Dialog::information(QString s) {
    _widget->information(s);
}

void Dialog::messageImpl(QString s) {
    _widget->message(s);
}

void Dialog::errorImpl(QString s) {
    _widget->errorImpl(s);
}

}
