/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "dialog_quiet.h"

namespace base {

DialogQuiet::DialogQuiet(QObject *parent)
    : DialogBase(parent)
{
}

void DialogQuiet::init() {
}

void DialogQuiet::progress(int, int) {
}

void DialogQuiet::finished() {
}

QProgressBar* DialogQuiet::progressBar() {
    return nullptr;
}

QWinTaskbarProgress* DialogQuiet::winProgressTaskbar() {
    return nullptr;
}

void DialogQuiet::finishProgress() {
}

void DialogQuiet::information(QString s) {
    _information = s;
}

void DialogQuiet::messageImpl(QString) {
}

void DialogQuiet::errorImpl(QString s) {
    _error = s;
}

QString DialogQuiet::getInformation() const {
    return _information;
}

QString DialogQuiet::getError() const {
    return _error;
}

}
