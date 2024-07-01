/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <QApplication>
#include <QLabel>
#include <QProgressBar>
#include <QProgressDialog>
#include <QScreen>
#include "dialog_minimal.h"
#include "environment.h"
#include "win_taskbar_progress.h"

namespace base {

DialogMinimal::DialogMinimal(QObject *parent)
    : DialogQuiet(parent)
{
    _progressDialog = new QProgressDialog;
    _progressDialog->setWindowModality(Qt::WindowModal);
    _progressDialog->setLabel(new QLabel("Running " + environment().currentLoadArg() + "...", _progressDialog));
    _progressDialog->setBar(_progressBar = new QProgressBar);

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect rect = screen->availableGeometry();
    int W = rect.width();
    QSize size = _progressDialog->size();
    _progressDialog->resize(W/4, size.height());

    _progressDialog->show();
}

QProgressBar* DialogMinimal::progressBar() {
    Q_ASSERT(_progressBar);
    return _progressBar;
}

void DialogMinimal::finishProgress() {
    _progressDialog->close();
}

}
