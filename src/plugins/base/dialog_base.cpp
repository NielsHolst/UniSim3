/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QApplication>
#include <QProgressBar>
#include <QTextEdit>
#include "dialog_base.h"
#include "exception.h"
#include "history.h"

namespace base {

DialogBase* DialogBase::_dialog = nullptr;
QStack<QString> DialogBase::_messages;

DialogBase& dialog() {
    Q_ASSERT(DialogBase::_dialog);
    return *DialogBase::_dialog;
}

DialogBase::DialogBase(QObject *parent)
    : QObject(parent),
      _errorCount(0)
{
    _dialog = this;
    resetProgress();
}

void DialogBase::resetProgress() {
    _nextShowProgress = 0.01;
}

void DialogBase::updateProgress(const ProgressInfo &info) {
    double progress = double(info.step + (info.iteration-1)*info.steps)/info.steps/info.iterations;
    if (progress > _nextShowProgress) {
        double total = static_cast<double>(info.time.elapsed())/progress;
        int maximum = static_cast<int>(total)/1000,
            value = static_cast<int>(info.time.elapsed())/1000;
        QProgressBar *bar = progressBar();
        if (bar) {
            bar->setFormat(" %p% of %ms");
            bar->setMaximum(maximum);
            bar->setValue(value);
            bar->show();
        }
        #ifdef Q_OS_WIN
          #if QT_VERSION < 0x060000
              QWinTaskbarProgress *taskbar = winProgressTaskbar();
              if (taskbar) {
                  taskbar->setMaximum(maximum);
                  taskbar->setValue(value);
                  taskbar->show();
              }
          #endif
        #endif
        qApp->processEvents();
        _nextShowProgress += 0.01;
    }
}

QTextEdit* DialogBase::textEdit() {
    return nullptr;
}

const History* DialogBase::history() const {
    return nullptr;
}

void DialogBase::message(QString s) {
    _message = s;
    messageImpl(s);
}

void DialogBase::pushMessage() {
    _messages.push(_message);
}

void DialogBase::popMessage() {
    if (_messages.isEmpty())
        ThrowException("Unexpected empty message stack");
    message(_messages.pop());
}

void DialogBase::error(QString s) {
    ++_errorCount;
    errorImpl(s);
}

void DialogBase::resetErrorCount() {
    _errorCount = 0;
}

int DialogBase::errorCount() const {
   return _errorCount;
}

QString DialogBase::latestCommand() const {
    QString s = dialog().history()->previous();
    auto items = s.trimmed().split(" ");
    return items.isEmpty() ? "" : items.at(0);
}

}

