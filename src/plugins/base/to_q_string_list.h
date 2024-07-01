/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef BASE_TO_Q_STRING_LIST_H
#define BASE_TO_Q_STRING_LIST_H

#if QT_VERSION >= 0x050E00
    inline QStringList toQStringList(QVector<QString> v) {return QStringList( QList<QString>(v.begin(), v.end()) ); }
#else
    inline QStringList toQStringList(QVector<QString> v) {return  QStringList( QList<QString>(v.toList()) ); }
#endif

#endif
