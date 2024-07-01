/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>

namespace base {
    class Dialog;
}

class MainWindow : public QMainWindow
{

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void init();
private:
    // Date
    base::Dialog *_dialog;
    // Methods
    void restore();
};

#endif
