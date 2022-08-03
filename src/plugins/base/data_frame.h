/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_DATA_FRAME_H
#define BASE_DATA_FRAME_H

#include <QStringList>
#include <QVector>
#include "convert.h"
#include "table.h"

namespace base {

class DataFrame : public Table
{
public:
    DataFrame(Node *parent = nullptr);
    void read(QString fileName, Labelling labelling);
    int numRow() const;
    int numCol() const;
    template <class T=QString> QVector<T> row(int i) const;
    template <class T=QString> QVector<T> col(int i) const;
    template <class T=QString> T at(int row, int col) const;
    template <class T=QString> T operator()(int row, int col) const;
    template <class T=QString> QVector<T> row(QString rowName) const;
    template <class T=QString> QVector<T> col(QString colName) const;
    template <class T=QString> QVector<QVector<T>> rows() const;
private:
    // Data
    QVector<QStringList> _rows;
};

template <class T>
QVector<T> DataFrame::row(int i) const {
    return convert<T, QVector>( _rows.at(i) );
}

template <class T>
QVector<T> DataFrame::col(int i) const {
    QVector<T> co;
    for (QStringList row : _rows) co << convert<T>( row.at(i) );
    return co;
}

template <class T>
T DataFrame::at(int row, int col) const {
    return convert<T>( _rows.at(row).at(col) );
}

template <class T>
T DataFrame::operator()(int row, int col) const {
    return at(row,col);
}

template <class T>
QVector<T> DataFrame::row(QString rowName) const {
    return row<T>( ixRow(rowName) );
}

template <class T>
QVector<T> DataFrame::col(QString colName) const {
    return col<T>( ixCol(colName) );
}

template <class T>
QVector<QVector<T>> DataFrame::rows() const {
    QVector<QVector<T>> all;
    for (QStringList row : _rows) all << convert<T, QVector>( row );
    return all;
}



} //namespace

#endif

