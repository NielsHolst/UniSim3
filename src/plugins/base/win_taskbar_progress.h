/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef WIN_TASKBAR_PROGRESS_H
#define WIN_TASKBAR_PROGRESS_H

#include <QObject>

#ifdef Q_OS_WIN
    #if QT_VERSION < 0x060000
        #include <QtWinExtras/QWinTaskbarProgress>
        #include <QtWinExtras/QWinTaskbarButton>
    #else
        #define QWinTaskbarProgress QObject
    #endif
#endif

#ifndef Q_OS_WIN
    #define QWinTaskbarProgress QObject
#endif

#endif
