/* Copyright 2005-2024 by
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

namespace base {
    class Box;
    class Node;
}

namespace command {

QString info(QVector<base::Node*> nodes);
QString info(QVector<base::Box *> boxes);
QString info(base::Node *node);

QString readHeader(QFile &file);
QStringList readHeadLines(QFile &file, int n);
QStringList readTailLines(QFile &file, int n);
void skipLines(QFile &file, int n);
QStringList readLines(QFile &file, int n);
QVector<int> getWidths(QStringList lines);
QStringList rightJustify(QStringList lines, QVector<int> widths);


}

#endif
