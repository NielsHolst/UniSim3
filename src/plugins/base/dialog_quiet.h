/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef BASE_DIALOG_QUIET_H
#define BASE_DIALOG_QUIET_H

#include "dialog_base.h"
#include "base_dllexport.h"
#include "win_taskbar_progress.h"

namespace base {

class UNI_API DialogQuiet : public DialogBase
{
public:
    DialogQuiet(QObject *parent);
    void init();
    void progress(int current, int total);
    void finished();
    QProgressBar* progressBar();
    void finishProgress();
    void information(QString s);
    QString getInformation() const;
    QString getError() const;
private:
    QString _message, _information, _error;
    void errorImpl(QString s);
    void messageImpl(QString s);
};

}

#endif
