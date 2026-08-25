/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef COMMAND_GENERAL
#define COMMAND_GENERAL

#include <boost/circular_buffer.hpp>

#include <QFile>
#include <QStringList>
#include <QVector>

#include <base/base_dllexport.h>

namespace base {
    class Box;
    class Node;
}

namespace command {

UNI_API QString info(QVector<base::Node*> nodes);
UNI_API QString info(QVector<base::Box *> boxes);
UNI_API QString info(base::Node *node);

UNI_API QString readHeader(QFile &file);
UNI_API QStringList readHeadLines(QFile &file, int n);
UNI_API QStringList readTailLines(QFile &file, int n);
UNI_API void skipLines(QFile &file, int n);
UNI_API QStringList readLines(QFile &file, int n);
UNI_API QVector<int> getWidths(QStringList lines);
UNI_API QStringList rightJustify(QStringList lines, QVector<int> widths);

}

#endif
