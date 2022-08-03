/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QMapIterator>
#include <QVector>
#include "exception.h"
#include "table.h"

namespace base {

Table::Table(Node *parent)
    : Node("Table", parent)
{
}

void Table::noNames() {
    _rowNamesLookup.clear();
    _colNamesLookup.clear();
}

QStringList Table::colNames() const {
    return QStringList(_colNames);
}

QStringList Table::rowNames() const {
    return QStringList(_rowNames);
}

void Table::setNames(QStringList rowNames, QStringList colNames) {
    int i;
    _rowNamesLookup.clear();
    _colNamesLookup.clear();
    _rowNames = rowNames;
    _colNames = colNames;

    i = 0;
    for (QString name : rowNames.toVector())  _rowNamesLookup[name] = i++;
    i = 0;
    for (QString name : colNames.toVector())  _colNamesLookup[name] = i++;
}

int Table::lookup(const QMap<QString,int> &names, QString name, QString direction) const {
    QMap<QString,int>::const_iterator it = names.constFind(name);
    if (it == names.constEnd())
        ThrowException("No " + direction + " with that name").value(name).context(this)
                .hint("Valid names:\n" + QStringList(names.keys()).join("\n"));
    return it.value();
}

int Table::ixRow(QString rowName) const {
    if (_rowNamesLookup.isEmpty())
        ThrowException("Table has no row names");
    return lookup(_rowNamesLookup, rowName, "row");
}

int Table::ixCol(QString colName) const {
    if (_colNamesLookup.isEmpty())
        ThrowException("Table has no column names");
    return lookup(_colNamesLookup, colName, "column");
}

} //namespace


