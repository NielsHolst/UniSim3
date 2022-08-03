/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_TABLE_H
#define BASE_TABLE_H

#include <QMap>
#include "node.h"
#include <QStringList>

namespace base {

class Table : public Node
{
public:
    enum Labelling {ColumnLabelled, RowLabelled, BothLabelled, NoLabelling};
    Table (Node *parent = nullptr);
    virtual void read(QString fileName, Labelling labelling) = 0;
    virtual int numRow() const = 0;
    virtual int numCol() const = 0;
    void noNames();
    QStringList rowNames() const;
    QStringList colNames() const;
    void setNames(QStringList rowNames, QStringList colNames);
    int ixRow(QString rowName) const;
    int ixCol(QString colName) const;
protected:
    // Data
    QMap<QString,int> _rowNamesLookup, _colNamesLookup;
    QStringList _rowNames, _colNames;
private:
    // Methods
    int lookup(const QMap<QString,int> &names, QString name, QString direction) const;
};


} //namespace

#endif

