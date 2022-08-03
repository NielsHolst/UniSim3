/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_DIALOG_MINIMAL_H
#define BASE_DIALOG_MINIMAL_H

#include "dialog_quiet.h"

class QProgressBar;
class QProgressDialog;

namespace base {

class DialogMinimal : public DialogQuiet
{
public:
    DialogMinimal(QObject *parent);
    QProgressBar* progressBar();
    QWinTaskbarProgress* winProgressTaskbar();
    void finishProgress();
private:
    QProgressBar *_progressBar;
    QProgressDialog *_progressDialog;
};

}

#endif
