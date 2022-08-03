/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_MATRIX_H
#define BASE_MATRIX_H

#include <QTextStream>
#include <QVector>
#include "convert.h"
#include "data_frame.h"
#include "table.h"
#include "vector_op.h"

namespace base {

template <class T>
class Matrix : public Table
{
public:
    Matrix(Node *parent = nullptr);
    void read(QString fileName, Labelling labelling);
    void resize(int rows, int cols);
    int numRow() const;
    int numCol() const;
    void fill(T value);
    QVector<T> row(int i) const;
    QVector<T> col(int i) const;
    T at(int row, int col) const;
    T& operator()(int row, int col);
    QVector<T> row(QString rowName) const;
    QVector<T> col(QString colName) const;
    QVector<QVector<T>> rows() const;

    T rowSum(int i) const;
    T colSum(int i) const;

    QString toString() const;

private:
    // Data
    DataFrame *_df;
    QVector<QVector<T>> _rowsTyped;
    int _nrow, _ncol;
};

template <class T>
Matrix<T>::Matrix(Node *parent)
    : Table(parent)
{
    setObjectName("Matrix");
    _df = new DataFrame(this);
}

template <class T>
void Matrix<T>::read(QString fileName, Labelling labelling) {
    _df->read(fileName, labelling);
    _nrow = _df->numRow();
    _ncol = _df->numCol();

    _rowsTyped.clear();
    for (int i=0; i<_nrow; ++i) {
        QStringList items = QStringList( _df->row<QString>(i).toList() );
        _rowsTyped << convert<T, QVector>(items);
    }
}

template <class T>
void Matrix<T>::resize(int rows, int cols) {
    _nrow = rows;
    _ncol = cols;
    _rowsTyped.clear();
    for (int i=0; i<_nrow; ++i) {
        _rowsTyped << QVector<T>(_ncol);
    }
    noNames();
}

template <class T>
int Matrix<T>::numRow() const {
    return _nrow;
}

template <class T>
int Matrix<T>::numCol() const {
    return _ncol;
}

template <class T>
void Matrix<T>::fill(T value) {
    for (QVector<T> &row : _rowsTyped)
        row.fill(value);
}

template <class T>
QVector<T> Matrix<T>::row(int i) const {
    return _rowsTyped.at(i);
}

template <class T>
QVector<T> Matrix<T>::col(int i) const {
    QVector<T> co;
    co << _rowsTyped.at(i);
    return co;
}

template <class T>
T Matrix<T>::at(int row, int col) const {
    return _rowsTyped.at(row).at(col);
}

template <class T>
T& Matrix<T>::operator()(int row, int col) {
    return _rowsTyped[row][col];
}

template <class T>
QVector<T> Matrix<T>::row(QString rowName) const {
    return row( ixRow(rowName) );
}

template <class T>
QVector<T> Matrix<T>::col(QString colName) const {
    return col( ixCol(colName) );
}

template <class T>
QVector<QVector<T>> Matrix<T>::rows() const {
    return _rowsTyped;
}

template <class T>
T Matrix<T>::rowSum(int i) const {
    QVector<T> v = row(i);
    return vector_op::sum(v);
}

template <class T>
T Matrix<T>::colSum(int i) const {
    QVector<T> v = col(i);
    return vector_op::sum(v);
}

//template <class T>
//T Matrix<T>::rowSum(int) const {
//    ThrowException("Matrix<Y>:rowSum only allowed for Matrix<double>");
//}

//template <class T>
//T Matrix<T>::colSum(int) const {
//    ThrowException("Matrix<Y>:colSum only allowed for Matrix<double>");
//}

template <class T>
QString Matrix<T>::toString() const {
    QString string;
    QTextStream s(&string);
    for (QVector<T> row : _rowsTyped) {
        bool first(true);
        for (T col : row) {
            if (!first) s << "\t";
            s << col;
            first = false;
        }
        s << "\n";
    }
    return string;
}

} //namespace

#endif

